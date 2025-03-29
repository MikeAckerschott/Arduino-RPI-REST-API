// DISTANCE SENSORS
#include <VL53L0X.h>
#include <HCSR04.h>
#include <Wire.h>

const byte triggerPin = 7;
const byte echoPin = 6;

HCSR04 hc(triggerPin, echoPin);
VL53L0X sensor;

void setupDistanceSensors() {
  Wire.begin();

  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  sensor.startContinuous();
}

void getDistances(int *distanceSensor1, int *distanceSensor2) {
  *distanceSensor1 = sensor.readRangeContinuousMillimeters();
  *distanceSensor2 = hc.dist();
}

void updateSensorData() {
  // READ SENSORS
  int sensor1 = 0;
  int sensor2 = 0;
  getDistances(&sensor1, &sensor2);

  // INSERT INTO BUFFER
  insert_buffer(&buffer1, sensor1);
  insert_buffer(&buffer2, sensor2);

  // UPDATE RUNNING MEAN AND STANDARD DEVIATION
  totalReadings++;
  totalSensor1 += sensor1;
  totalSensor2 += sensor2;
  totalSquaredSensor1 += sensor1 * sensor1;
  totalSquaredSensor2 += sensor2 * sensor2;
  averageSensor1 = totalSensor1 / static_cast<double>(totalReadings);
  averageSensor2 = totalSensor2 / static_cast<double>(totalReadings);

  stdDevSensor1 = sqrt((totalSquaredSensor1 - totalSensor1 * totalSensor1 / totalReadings) / static_cast<double>(totalReadings - 1));
  stdDevSensor2 = sqrt((totalSquaredSensor2 - totalSensor2 * totalSensor2 / totalReadings) / static_cast<double>(totalReadings - 1));

  if (totalReadings % 100 == 0) {
    Serial.println("Sensor data updated 100 times.");
  }

  previousSensorReading = millis();
}