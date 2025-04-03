#include <Wire.h>
#include <VL53L0X.h>
#include <HCSR04.h>


byte triggerPin = 7;
byte echoPin = 6;
VL53L0X sensor;

void setupDistanceSensors() {

  HCSR04.begin(triggerPin, echoPin);
  Wire.begin();

  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  sensor.startContinuous();
}

void getDistances(int* distanceSensor1, int* distanceSensor2) {
  *distanceSensor1 = sensor.readRangeContinuousMillimeters();
  double* distances = HCSR04.measureDistanceMm();
  *distanceSensor2 = distances[0];
}

void setup() {
  Serial.begin(9600);
  setupDistanceSensors();
}

void loop() {
  int sensor1 = 0;
  int sensor2 = 0;

  getDistances(&sensor1, &sensor2);

  Serial.print(sensor1);
  Serial.print(" , ");
  Serial.println(sensor2);
}