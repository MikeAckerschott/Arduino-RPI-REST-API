
#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Hello!");

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  dac.begin(0x60);

}

void loop(void)
{
  PINB = _BV(PINB5);
  PINB = _BV(PINB5); 
  dac.setVoltage(2025, false);

}