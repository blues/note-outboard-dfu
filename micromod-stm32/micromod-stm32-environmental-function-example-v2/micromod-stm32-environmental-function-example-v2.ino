#include <Arduino.h>
#include <Wire.h>
#include <Notecard.h>
#include "metadata.h"

#include "SparkFun_SHTC3.h"
#include "SparkFun_SGP40_Arduino_Library.h"

#define serialDebugOut Serial

// Change to use env vars
#define UPDATE_PERIOD (1000 * 60 * 5)

#include "SparkFun_SHTC3.h"
SHTC3 SHTC3sensor;

#include "SparkFun_SGP40_Arduino_Library.h"
SGP40 SGP40sensor;

#include "SparkFun_STC3x_Arduino_Library.h"
STC3x STC3xSensor;

Notecard notecard;

// Variables for Env Var polling
static unsigned long nextPollMs = 0;
static unsigned long lastUpdateMs;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);

    serialDebugOut.begin(115200);
    delay(2500);
    notecard.setDebugOutputStream(serialDebugOut);

    Wire.begin();
    notecard.begin();

    J *req = notecard.newRequest("hub.set");
    if (req != NULL) {
      JAddStringToObject(req, "mode", "continuous");
      JAddStringToObject(req, "sn", "stm32-micromod");
      notecard.sendRequest(req);
    }

    req = notecard.newRequest("dfu.status");
    if (req != NULL) {
      JAddStringToObject(req, "version", firmwareVersion());
      notecard.sendRequest(req);
    }

    req = notecard.newRequest("card.dfu");
    if (req != NULL) {
      // Use "stm32-bi" to target the SparkFun MicroMod STM32
      JAddStringToObject(req, "name", "stm32-bi");
      JAddBoolToObject(req, "on", true);
      notecard.sendRequest(req);
    }

    if (SHTC3sensor.begin() != SHTC3_Status_Nominal)
    {
      Serial.println(F("SHTC3 not detected. Please check wiring. Freezing..."));
      while (1)
        ; // Do nothing more
    }
  
    if (SGP40sensor.begin() == false)
    {
      Serial.println(F("SGP40 not detected. Check connections. Freezing..."));
      while (1)
        ; // Do nothing more
    }

    if (STC3xSensor.begin() == false)
    {
      Serial.println(F("STC3x not detected. Please check wiring. Freezing..."));
      while (1)
        ; // Do nothing more
    }

    if (STC3xSensor.setBinaryGas(STC3X_BINARY_GAS_CO2_AIR_25) == false)
    {
      Serial.println(F("Could not set the binary gas! Freezing..."));
      while (1)
        ; // Do nothing more
    }

    float temperature = SHTC3sensor.toDegC();
    Serial.print(F("Setting STC3x temperature to "));
    Serial.print(temperature, 2);
    Serial.print(F("C was "));
    if (STC3xSensor.setTemperature(temperature) == false)
      Serial.print(F("not "));
    Serial.println(F("successful"));
  
    float relativeHumidity = SHTC3sensor.toPercent();
    Serial.print(F("Setting STC3x RH to "));
    Serial.print(relativeHumidity, 2);
    Serial.print(F("% was "));
    if (STC3xSensor.setRelativeHumidity(relativeHumidity) == false)
      Serial.print(F("not "));
    Serial.println(F("successful"));
  
    uint16_t pressure = 840;
    Serial.print(F("Setting STC3x pressure to "));
    Serial.print(pressure);
    Serial.print(F("mbar was "));
    if (STC3xSensor.setPressure(pressure) == false)
      Serial.print(F("not "));
    Serial.println(F("successful"));
}

void loop()
{
    const uint32_t currentMillis = millis();

    if (currentMillis - lastUpdateMs >= UPDATE_PERIOD)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        lastUpdateMs = currentMillis;
        Serial.println("Sending sensor readings to the Notecard...");
        sendSensorReadings();

        digitalWrite(LED_BUILTIN, LOW);
    }
}

void sendSensorReadings()
{
  SHTC3_Status_TypeDef result = SHTC3sensor.update();

  float relativeHumidity = SHTC3sensor.toPercent();
  Serial.print(F("Relative Humidity = "));
  Serial.print(relativeHumidity);

  float temperature = SHTC3sensor.toDegF();
  Serial.print(temperature);
  Serial.print(F(" deg F"));

  if (SHTC3sensor.lastStatus == SHTC3_Status_Nominal)
  {
    Serial.println("");                                        
  }
  else {
    Serial.print(F(",     Update failed, error: "));
    errorDecoder(SHTC3sensor.lastStatus);
    Serial.println("");
  }

  float vocIndex = SGP40sensor.getVOCindex();
  Serial.print(F("VOC Index is: "));
  Serial.println(vocIndex);

  if (STC3xSensor.setRelativeHumidity(relativeHumidity) == false)
    Serial.print(F("Unable to set STC31 Relative Humidity with SHTC3."));

  if (STC3xSensor.setTemperature(temperature) == false)
    Serial.println(F("Unable to set STC31 Temperature with SHTC3."));


  Serial.print(F("CO2(%): "));

  if (STC3xSensor.measureGasConcentration())                   // measureGasConcentration will return true when fresh data is available
  {
    Serial.println(STC3xSensor.getCO2(), 2);
  }
  else
  {
    Serial.print(STC3xSensor.getCO2(), 2);
    Serial.println(F(",     (old STC3 sample reading, STC31 was not able to get fresh data yet)"));  //output this note to indicate  when we are not able to obtain a new measurement
  }

  float co2 = STC3xSensor.getCO2();

  J *req = notecard.newRequest("note.add");
  if (req != NULL)
  {
    JAddBoolToObject(req, "sync", true);
    JAddStringToObject(req, "file", "readings.qo");
    J *body = JCreateObject();
    if (body != NULL)
    {
      JAddNumberToObject(body, "humidity", relativeHumidity);
      JAddNumberToObject(body, "temperature", temperature);
      JAddNumberToObject(body, "voc_index", vocIndex);
      JAddNumberToObject(body, "co2", co2);
      JAddItemToObject(req, "body", body);
      notecard.sendRequest(req);
    }
  }
}

void errorDecoder(SHTC3_Status_TypeDef message)
{
  switch (message)
  {
    case SHTC3_Status_Nominal : Serial.print("Nominal"); break;
    case SHTC3_Status_Error : Serial.print("Error"); break;
    case SHTC3_Status_CRC_Fail : Serial.print("CRC Fail"); break;
    default : Serial.print("Unknown return code"); break;
  }
}
