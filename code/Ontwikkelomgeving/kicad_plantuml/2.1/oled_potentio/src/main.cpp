#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define POT_PIN A0
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Scherm niet gevonden"));
    while (true);
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Potentiometer Stand:"));
  display.display();
  delay(2000);
}

void loop() {
  int potValue = analogRead(POT_PIN);
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println(potValue);
  display.display();
}
