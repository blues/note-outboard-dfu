#include <Arduino.h>
#include <Notecard.h>

#define serialDebugOut Serial

Notecard notecard;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);

    serialDebugOut.begin(115200);
    notecard.setDebugOutputStream(serialDebugOut);

    Wire.begin();
    notecard.begin();

    J *req = notecard.newRequest("hub.set");
    JAddStringToObject(req, "mode", "continuous");
    JAddStringToObject(req, "sn", "stm32-feather-v1");
    notecard.sendRequest(req);

    req = notecard.newRequest("card.dfu");
    JAddStringToObject(req, "name", "stm32");
    JAddBoolToObject(req, "on", true);
    // On Notecarrier F the DFU signals are routed over the shared AUX pins.
    JAddStringToObject(req, "mode", "aux");
    notecard.sendRequest(req);

    // Free the AUX pins so they can be used for Outboard Firmware Update.
    req = notecard.newRequest("card.aux");
    JAddStringToObject(req, "mode", "off");
    notecard.sendRequest(req);

    // Enable host DFU and report the running firmware version to Notehub.
    req = notecard.newRequest("dfu.status");
    JAddBoolToObject(req, "on", true);
    JAddStringToObject(req, "version", "1.0.0");
    notecard.sendRequest(req);
}

void loop()
{
    Serial.println("Hello, world!");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}
