#include <Notecard.h>
#include <Wire.h>

// Hardware DFU stress test example application.
// Assumptions:
// Notecard is pre-configured with hostUID and projectUID.
// Notecard is pre-configured in continuous or periodic mode.
// Notehub project is pre-loaded with a list of other hostDFU binaries compatible with this host hardware in env var host_binary_list.

#define serialDebug Serial

// Define the pin number of the pushbutton pin
#if defined(ARDUINO_ARCH_ESP32)
#define ledPin        13
#elif defined(ARDUINO_ARCH_STM32)
#define ledPin        LED_BUILTIN
#else
#error "Hardware DFU only supported on STM32 and ESP32"
#endif

// Notecard I2C port definitions
Notecard notecard;

typedef enum {
    STATE_BOOT = 0,
    STATE_SYNC_WAIT1,
    STATE_READ_ENV_BINARY_LIST,
    STATE_WAIT_DFU_COMPLETION,
    STATE_TIMEOUT_THEN_RETRY,
} host_state_t;


host_state_t host_state = STATE_BOOT;
int reboots = 0;
unsigned long host_state_time = 0;

void setup()
{
    // Arduino IDE requires a delay to move the serial port over
    // from programming the MCU to the debug monitor.
    delay(2500);
    serialDebug.begin(115200);
    notecard.setDebugOutputStream(serialDebug);

    // Initialize the physical I2C I/O channel to the Notecard
    Wire.begin();
    notecard.begin();

    host_state_t host_state = STATE_BOOT;
    host_state_time = millis();

    // Initialize Arduino GPIO pins
    pinMode(ledPin, OUTPUT);
}

void loop()
{

    static bool rand_initialized = false;

    unsigned long state_age_s = (millis() - host_state_time) / 1000;

    char diagstring[100];

    sprintf(diagstring, "State %d age %ds\n", host_state, state_age_s);
    notecard.logDebug(diagstring);

    // flash LED to show the state
    int i, total_delay = 0;
    for (i = 0; i < host_state+1; i++) {
        digitalWrite(ledPin, HIGH);
        delay(200);
        digitalWrite(ledPin, LOW);
        delay(200);
        total_delay += 400;
    }

    delay(5000-total_delay);

    // Wait 60 seconds after reboot so that Notecard can sync result of prior DFU
    if (millis() < 60000) {
        return;
    }

    switch(host_state) {

    case STATE_BOOT: {
        J *req;
        bool error_occurred = false;

        // Zero out the host DFU state in the Notecard, so that if there is a host
        // binary waiting it does not immediately start hardware DFU.
        req = NoteNewRequest("dfu.status");
        if (req == NULL) {
            error_occurred = true;
        } else {
            JAddBoolToObject(req, "stop", true);
            if (!NoteRequest(req)) {
                error_occurred = true;
            }
        }

        // clear the DFU variables to prevent a DFU from starting before we expect
        if (!NoteSetEnvDefault("_fw", "") || !NoteSetEnvDefault("_fw_retry", "")) {
            error_occurred = true;
        }

        // Configure the Notecard for hardware DFU appropriate to this host
        req = NoteNewRequest("card.dfu");
        if (req == NULL) {
            error_occurred = true;
        } else {
#if defined(ARDUINO_ARCH_STM32)
            JAddStringToObject(req, "name", "stm32");
#endif
#if defined(ARDUINO_ARCH_ESP32)
            JAddStringToObject(req, "name", "esp32");
#endif
            if (!NoteRequest(req)) {
                error_occurred = true;
            }
        }

        if (!error_occurred && !rand_initialized) {
            // Use the Notecard card.random to see the Arduino random number generator,
            // defaulting to using the ADC input.
            int randomseed = analogRead(0);
            req = NoteNewRequest("card.random");
            if (req == NULL) {
                error_occurred = true;
            } else {
                JAddNumberToObject(req, "count", 100000);
                J *rsp = NoteRequestResponse(req);
                int i = JGetInt(rsp, "count");
                if (i > 0) {
                    randomseed = i;
                }
                JFree(rsp);
            }
            sprintf(diagstring, "Random seed %d\n", randomseed);
            notecard.logDebug(diagstring);
            randomSeed(randomseed);
            rand_initialized = true;
        }

        // Add a note that uniquely identifies this host firmware and then sync to notehub
        req = NoteNewRequest("note.add");
        if (req == NULL) {
            error_occurred = true;
        } else {
            JAddStringToObject(req, "file", "directdfu.qo");
            J *body = JCreateObject();
            if (body == NULL) {
                error_occurred = true;
                JFree(req);
            } else {
                JAddStringToObject(body, "host", "arduino");
                JAddNumberToObject(body, "retries", reboots);
                JAddItemToObject(req, "body", body);
            }
            if (NoteRequest(req)) {
                req = NoteNewRequest("hub.sync");
                if (req != NULL) {
                    if (NoteRequest(req)) {

                        host_state = STATE_SYNC_WAIT1;
                        host_state_time = millis();
                    }
                }
            }
        }
        if (error_occurred) {
            notecard.logDebug("!!! ERROR! Wait and retry  !!!\n");
            host_state = STATE_TIMEOUT_THEN_RETRY;
            host_state_time = millis();
        }
    }
    break;

    // Wait for the sync to complete, as indicated by file.changes.pending empty response
    case STATE_SYNC_WAIT1: {
        J *req = NoteNewRequest("file.changes.pending");
        if (req != NULL) {
            J *rsp = NoteRequestResponse(req);
            if (rsp != NULL) {
                if (!JGetBool(rsp, "pending")) {
                    host_state = STATE_READ_ENV_BINARY_LIST;
                    host_state_time = millis();
                }
                JFree(rsp);
            }
        }
        if (state_age_s > 300) {
            notecard.logDebug("!!! SYNC TIMEOUT !!!\n");
            host_state = STATE_TIMEOUT_THEN_RETRY;
            host_state_time = millis();
        }
    }
    break;

    // Read the list of host binaries from the notecard and select one at random
    case STATE_READ_ENV_BINARY_LIST: {
        char host_binaries[150];
        host_binaries[0] = '\0';
        if (!NoteGetEnv("host_binary_list", NULL, host_binaries, sizeof(host_binaries))) {
            notecard.logDebug("!!! ERROR READING host_binary_list !!!\n");
            host_state = STATE_TIMEOUT_THEN_RETRY;
            host_state_time = millis();
        } else if (strlen(host_binaries) == 0) {
            host_state = STATE_TIMEOUT_THEN_RETRY;
            host_state_time = millis();
            notecard.logDebug("!!! host_binary_list is empty !!!\n");
        } else {

            int  numbins = 1;
            char *binname;
            long random_number;

            notecard.logDebug("Host binaries:");
            notecard.logDebug(host_binaries);
            notecard.logDebug("\n");

            // How many binaries are listed
            binname = host_binaries;
            while (*binname != '\0') {
                if (*binname == ',') {
                    numbins++;
                }
                binname++;
            }

            if (numbins > 1) {
                char separator[2];
                separator[0] = ',';
                separator[1] = '\0';

                random_number = random(0, numbins-1);
                sprintf(diagstring, "Selecting %d of %d binaries\n", random_number+1, numbins);
                notecard.logDebug(diagstring);

                binname = strtok(host_binaries, separator);
                while (random_number--) {
                    binname = strtok(NULL, separator);
                }
                if (binname == NULL) {
                    notecard.logDebug("!!! OOPS STRTOK FAIL !!!\n");
                    binname = host_binaries;
                }
            } else {
                binname = host_binaries;
            }

            // Set environment variables for DFU
            char randstring[20];
            sprintf(randstring, "%d", random(0,1000));
            if (!NoteSetEnvDefault("_fw", binname) ||
                    !NoteSetEnvDefault("_fw_retry", randstring)) {
                notecard.logDebug("!!! env.default _fw_retry failed !!!\n");
                host_state = STATE_BOOT;
                host_state_time = millis();
                reboots++;
            } else {
                // Sync to kick start the DFU
                J *req = NoteNewRequest("hub.sync");
                if (req != NULL) {
                    NoteRequest(req);
                }
                host_state = STATE_WAIT_DFU_COMPLETION;
                host_state_time = millis();
            }
        }
    }
    break;

    // Read the list of host binaries from the notecard and select one at random
    case STATE_WAIT_DFU_COMPLETION: {
        J *req = NoteNewRequest("dfu.status");
        if (req != NULL) {
            J *rsp = NoteRequestResponse(req);
            // monitor the status of the DFU
            // perhaps bail if there's an error
            JFree(rsp);
        }
        req = NoteNewRequest("hub.sync.status");
        if (req != NULL) {
            J *rsp = NoteRequestResponse(req);
            // monitor the status of the sync
            // perhaps bail if there's an error
            JFree(rsp);
        }
        if (state_age_s > 1800) {
            notecard.logDebug("!!! DFU TIMEOUT !!!\n");
            host_state = STATE_BOOT;
            host_state_time = millis();
            reboots++;
        }
    }
    break;

    case STATE_TIMEOUT_THEN_RETRY:
        if (state_age_s > 600) {
            notecard.logDebug("!!! RETRY !!!\n");
            host_state = STATE_BOOT;
            host_state_time = millis();
            reboots++;
        }
        break;

    } // switch

}
