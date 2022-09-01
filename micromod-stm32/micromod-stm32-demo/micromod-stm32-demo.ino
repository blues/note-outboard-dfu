#include <Arduino.h>
#include <Notecard.h>

#define serialDebugOut Serial
#define PRODUCT_UID "com.blues.bsatrom:outboard_dfu_demo"

int ledPin = PA15;

Notecard notecard;

void setup()
{
    pinMode(ledPin, OUTPUT);
    Serial.begin(115200);

    serialDebugOut.begin(115200);
    notecard.setDebugOutputStream(serialDebugOut);

    Wire.begin();
    notecard.begin();

    J *req = notecard.newRequest("hub.set");
    JAddStringToObject(req, "product", PRODUCT_UID);
    JAddStringToObject(req, "mode", "continuous");
    JAddStringToObject(req, "sn", "stm3232-micromod-v1");
    notecard.sendRequest(req);

    req = notecard.newRequest("card.dfu");
    JAddStringToObject(req, "name", "stm32-bi");
    JAddBoolToObject(req, "on", true);
    notecard.sendRequest(req);
}

void loop()
{
    Serial.println("Hello, world!");
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);
}
