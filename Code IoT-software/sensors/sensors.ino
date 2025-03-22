#include "Adafruit_VL53L0X.h"
#include <HCSR04.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

byte triggerPin = 7;
byte echoPin = 6;

void setup() {
  Serial.begin(115200);
  
  while (!Serial) {
    delay(1);
  }
  
  Serial.println("Initializing sensors...");
  
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  
  HCSR04.begin(triggerPin, echoPin);
  
  Serial.println(F("Sensors initialized successfully."));
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    Serial.print("VL53L0X Distance: "); 
    Serial.print(measure.RangeMilliMeter);
    Serial.println(" mm");
  } else {
    Serial.println("VL53L0X: Out of range");
  }
  
  double* distances = HCSR04.measureDistanceCm();
  Serial.print("HCSR04 Distance: ");
  Serial.print(distances[0]);
  Serial.println(" cm");

  Serial.println("---");
  delay(10);
}
