#include <Arduino.h>

#define LED_PIN 13

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(LED_PIN, LOW);
}