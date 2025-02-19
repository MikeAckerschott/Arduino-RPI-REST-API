#include <Arduino.h>

#define POT_PIN A0

void setup() {
  Serial.begin(9600);
  while(!Serial){
    
  }
}

void loop() {
  Serial.println(analogRead(POT_PIN));
}
