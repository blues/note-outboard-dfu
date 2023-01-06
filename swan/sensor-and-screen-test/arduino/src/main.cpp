#include <Arduino.h>
#include <Notecard.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define BME680_ADDRESS 0x77

// pressure at the sea level in hectopascal (is equivalent to milibar)
#define SEALEVELPRESSURE_HPA (1013.25)

Notecard notecard;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_BME680 bmeSensor;

bool displayConnected = false;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 1000 * 60;

void setup() {
  Serial.begin(115200);
  delay(20000); // time to get serial monitor open!
  Serial.println("===============");
  Serial.println("Outboard DFU Test - Arduino!");
  Serial.println("===============");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Wire.begin();
  notecard.begin();

  J *req = notecard.newRequest("hub.set");
  JAddStringToObject(req, "sn", "arduino-notecard");
  JAddStringToObject(req, "mode", "continuous");
  notecard.sendRequest(req);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));

  } else {
    Serial.println("Screen Connected");
    displayConnected = true;
  }

  if (displayConnected) {
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Outboard DFU Test - Arduino!"));
    display.display();
  }

  unsigned bmeStatus = bmeSensor.begin(BME680_ADDRESS);
  if (!bmeStatus)
  {
    Serial.println("Could not find a valid BME680 sensor, check wiring, I2C address");
  }
  else
  {
    Serial.println("BME680 Connected");
  }

  digitalWrite(LED_BUILTIN, LOW);
  startMillis = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
}