#include "Adafruit_VL53L0X.h"
#include <HCSR04.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

byte triggerPin = 7;
byte echoPin = 6;

void setupDistanceSensors() {
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;
  }
  HCSR04.begin(triggerPin, echoPin);
}

void getDistances(int * distanceSensor1, int * distanceSensor2) {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  *distanceSensor1 = measure.RangeMilliMeter;
  *distanceSensor2 = HCSR04.measureDistanceCm();
}