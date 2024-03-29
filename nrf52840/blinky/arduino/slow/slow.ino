#include <Arduino.h>
#include <Notecard.h>

#ifndef NRF52840_XXAA
#error This example is for the Adafruit Feather nRF52840 Express only
#endif

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
    JAddBoolToObject(req, "sync", true);
    JAddStringToObject(req, "sn", "blinky-arduino-slow");
    notecard.sendRequest(req);

    req = notecard.newRequest("card.dfu");
    JAddStringToObject(req, "name", "nrf52");
    JAddBoolToObject(req, "on", true);
    notecard.sendRequest(req);
}

void loop()
{
    Serial.println("Hello, world! This is Arduino slow blinky.");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}
