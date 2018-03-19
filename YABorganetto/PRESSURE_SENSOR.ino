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
float prevPressure = 0;
float prevReadingTime = 0;

void readPressureSensor () {
  float newPressure = bme.readPressure();
  float newReadingTime = millis();

  if (pressure != newPressure) {
    prevReadingTime = readingTime;
    prevPressure = pressure;
    pressure = newPressure;
    readingTime = newReadingTime;
    //graphPressure();
  }
}

void setupPressureSensor () {
  int retryCount = 0;

  while (!bme.begin()) {
    retryCount++;

    if (retryCount >= 20) {
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
      Serial.println("Baseline measuring failure!");
    }
    else {
      measure += pressure;
      measureCount ++;
    }
    delay(25);
  }
  Serial.println("BaselinePressure done!");

  baselinePressure = measure / BASELINE_PRESSURE_READ_COUNT;
}

float getPressure () {
  return pressure;
}

float getPrevRelativePressure () {
  return baselinePressure - prevPressure;
}

float getRelativePressure () {
  return baselinePressure - pressure;
}

float getRelativePressurePrediction (int timePast) {
  float delta = ((prevPressure - pressure) / (readingTime - prevReadingTime)) * timePast;
  return baselinePressure - pressure + delta;
}

void graphPressure () {
    Serial.print("600 0 -600 40 -40 90 -90 ");
    Serial.print(constrain(getRelativePressurePrediction(50), -600, 600));
    Serial.print(" ");
    Serial.println(constrain(getRelativePressure(), -600, 600));
}
