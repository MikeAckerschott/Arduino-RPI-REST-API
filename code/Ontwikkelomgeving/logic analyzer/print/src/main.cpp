#include <Arduino.h>

void setup() {
    Serial.begin(9600);
    while (!Serial) { }
}

void loop() {
    Serial.print("print");
}
