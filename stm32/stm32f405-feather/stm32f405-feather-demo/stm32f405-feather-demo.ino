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
