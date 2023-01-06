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
    JAddStringToObject(req, "sn", "blinky-arduino-fast");
    notecard.sendRequest(req);

    req = notecard.newRequest("card.dfu");
    JAddStringToObject(req, "name", "nrf52");
    JAddBoolToObject(req, "on", true);
    notecard.sendRequest(req);
}

void loop()
{
    Serial.println("Hello, world! This is Arduino fast blinky.");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
}
