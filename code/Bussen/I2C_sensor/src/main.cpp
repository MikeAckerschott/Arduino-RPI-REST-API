#include "Adafruit_VL53L0X.h"
#include <Arduino.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
VL53L0X_RangingMeasurementData_t measure;

void setup() {
  Serial.begin(9600);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  pinMode(13, OUTPUT);
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  digitalWrite(13, HIGH);
  digitalWrite(13, LOW);

  // if(lox.isRangeComplete)
  lox.getRangingMeasurement(&measure);
}