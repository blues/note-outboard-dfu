#include <Arduino.h>
#include <Notecard.h>
#include <Wire.h>
#include <ButtonDebounce.h>

#define serialDebugOut Serial

Notecard notecard;
ButtonDebounce button(USER_BTN, 200);

uint8_t button_pressed_count = 0;
bool buttonTriggered = false;

void buttonChanged(const int state) {
  if (state == 1) {
    Serial.println("Button Verified: " + String(state));
    button_pressed_count++;

    buttonTriggered = true;
  }
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  delay(2500);
  serialDebugOut.begin(115200);
  notecard.setDebugOutputStream(serialDebugOut);

  Wire.begin();
  notecard.begin();

  J *req = notecard.newRequest("hub.set");
  JAddStringToObject(req, "mode", "continuous");
	JAddStringToObject(req, "sn", "arduino-notecard");
  notecard.sendRequest(req);

  req = notecard.newRequest("card.dfu");
  JAddStringToObject(req, "name", "stm32");
  JAddBoolToObject(req, "on", true);
  notecard.sendRequest(req);

  button.setCallback(buttonChanged);

  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  button.update();

  if (buttonTriggered) {
    digitalWrite(LED_BUILTIN, HIGH);
    buttonTriggered = false;

    J *req = notecard.newRequest("note.add");
    if (req != NULL) {
        JAddBoolToObject(req, "sync", true);
        J *body = JCreateObject();
        if (body != NULL) {
            JAddStringToObject(body, "os", "arduino");
          JAddNumberToObject(body, "button_count", button_pressed_count);
          JAddItemToObject(req, "body", body);
        }
        notecard.sendRequest(req);
    }

    digitalWrite(LED_BUILTIN, LOW);
  }
}