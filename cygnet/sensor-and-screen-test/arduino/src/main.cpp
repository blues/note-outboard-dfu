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

  // Enable Notecard Outboard Firmware Update for this STM32 host. On
  // Notecarrier F the DFU signals are routed over the shared AUX pins.
  req = notecard.newRequest("card.dfu");
  JAddStringToObject(req, "name", "stm32");
  JAddBoolToObject(req, "on", true);
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

    // Configure BME680 oversampling and filtering.
    bmeSensor.setTemperatureOversampling(BME680_OS_8X);
    bmeSensor.setHumidityOversampling(BME680_OS_2X);
    bmeSensor.setPressureOversampling(BME680_OS_4X);
    bmeSensor.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bmeSensor.setGasHeater(320, 150); // 320*C for 150 ms
  }

  digitalWrite(LED_BUILTIN, LOW);
  startMillis = millis();
}

void loop() {
  // Publish a sensor reading once per period, without blocking the loop.
  currentMillis = millis();
  if (currentMillis - startMillis < period) {
    return;
  }
  startMillis = currentMillis;

  if (!bmeSensor.performReading()) {
    Serial.println("Failed to read from BME680 sensor");
    return;
  }

  float temp = bmeSensor.temperature;
  float humidity = bmeSensor.humidity;
  float pressure = bmeSensor.pressure / 100.0;         // hPa
  float gas = bmeSensor.gas_resistance / 1000.0;       // KOhms

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" degrees C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  J *req = notecard.newRequest("note.add");
  if (req != NULL)
  {
    JAddStringToObject(req, "file", "sensors.qo");
    JAddBoolToObject(req, "sync", true);
    J *body = JCreateObject();
    if (body != NULL)
    {
      JAddNumberToObject(body, "temp", temp);
      JAddNumberToObject(body, "humidity", humidity);
      JAddNumberToObject(body, "pressure", pressure);
      JAddNumberToObject(body, "gas", gas);
      JAddItemToObject(req, "body", body);
    }
    notecard.sendRequest(req);
  }

  if (displayConnected)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.print(temp);
    display.println(" C");
    display.print("Humidity: ");
    display.print(humidity);
    display.println("%");
    display.display();
  }
}