#include <Arduino.h>

#define LED_PIN 13

void setup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    digitalWrite(LED_PIN, HIGH);
    Serial.print("LED is ON");

    delay(500);

    digitalWrite(LED_PIN, LOW);
    Serial.print("LED is OFF");
    delay(500);
}