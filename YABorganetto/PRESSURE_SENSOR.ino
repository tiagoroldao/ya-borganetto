#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

Adafruit_BMP280 bme;

const float MAX_BASELINE_PRESSURE_VARIANCE = 15;
const float BASELINE_PRESSURE_READ_COUNT = 25;
const float BASELINE_PRESSURE_READ_TIMER = 25;

const int PREV_PRESSURE_MIN_AGE = 5;

float baselinePressure;
// readings
float pressure = 0;
float readingTime = 0;
// Readings to compare to, for calculating momentum
// Always younger than PREV_PRESSURE_MIN_AGE * 2, always older than PREV_PRESSURE_MIN_AGE
float prevPressure = 0;
float prevReadingTime = 0;
// Reading that will replace old ones
// Replace readings when PREV_PRESSURE_MIN_AGE is passed
float prevPressureTemp = 0;
float prevReadingTimeTemp = 0;

void readPressureSensor () {
  pressure = bme.readPressure();
  readingTime = millis();

  if (readingTime - prevReadingTimeTemp > PREV_PRESSURE_MIN_AGE ||
      (pressure > baselinePressure && prevPressure < baselinePressure) ||
      (pressure < baselinePressure && prevPressure > baselinePressure)) {
    prevReadingTime = prevReadingTimeTemp;
    prevPressure = prevPressureTemp;
    prevPressureTemp = pressure;
    prevReadingTimeTemp = readingTime;
  }
}

void setupPressureSensor () {
  int retryCount = 0;

  while (!bme.begin()) {
    retryCount++;

    if (retryCount >= 10) {
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      while (1);
    }

    delay(100);
  }

  delay(100);

  float initPressure = bme.readPressure();
  int measure = initPressure;
  int measureCount = 1;

  while (measureCount < BASELINE_PRESSURE_READ_COUNT) {
    float pressure = bme.readPressure();
    if (abs(pressure - initPressure) > MAX_BASELINE_PRESSURE_VARIANCE) {
      initPressure = pressure;
      measure = pressure;
      measureCount = 1;
    }
    else {
      measure += pressure;
      measureCount ++;
    }
    delay(25);
  }

  baselinePressure = measure / BASELINE_PRESSURE_READ_COUNT;
  Serial.println("Successfully measured baseline pressure as " + String(baselinePressure));
}

float getPressure () {
  return pressure;
}

float getRelativePressure () {
  return baselinePressure - pressure;
}

float getRelativePressurePrediction (int timePast) {
//  Serial.println("prediction");
  float delta = (pressure - prevPressure) / (readingTime - prevReadingTime) * timePast;
  return baselinePressure - pressure + delta;
}
