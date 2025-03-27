#include <Wire.h>
#include <VL53L0X.h>
#include <HCSR04.h>

// AVERAGES AND STANDARD DEVIATIONS
unsigned long long totalReadings = 0;
unsigned long long totalSensor1 = 0;
unsigned long long totalSensor2 = 0;
unsigned long long totalSquaredSensor1 = 0;
unsigned long long totalSquaredSensor2 = 0;
double averageSensor1 = 0;
double averageSensor2 = 0;
double stdDevSensor1 = 0;
double stdDevSensor2 = 0;

// DISTANCE SENSORS
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

void updateSensorData(){
  // READ SENSORS
  int sensor1 = 0;
  int sensor2 = 0;
  getDistances(&sensor1, &sensor2);
  Serial.print("sensors read: ");
  Serial.print(sensor1);
  Serial.print(" , ");
  Serial.print(sensor2);

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

  stdDevSensor1 = sqrt((totalSquaredSensor1 - totalSensor1 * totalSensor1 / totalReadings) / static_cast<double>(totalReadings -1));
  stdDevSensor2 = sqrt((totalSquaredSensor2 - totalSensor2 * totalSensor2 / totalReadings) / static_cast<double>(totalReadings -1));

  Serial.print(" | average1: ");
  Serial.print(averageSensor1);
  Serial.print(" | average2: ");
  Serial.print(averageSensor2);
  Serial.print(" | stdDev1: ");
  Serial.print(stdDevSensor1);
  Serial.print(" | stdDev2: ");
  Serial.println(stdDevSensor2);

  previousSensorReading = millis();
}