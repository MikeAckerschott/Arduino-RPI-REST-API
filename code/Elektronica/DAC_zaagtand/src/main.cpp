#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

void setup(void)
{
  Serial.begin(9600);

  dac.begin(0x60);
}

void loop(void)
{
  uint32_t counter;
  for (counter = 0; counter < 4095; counter++)
  {
    dac.setVoltage(counter, false);
  }
  for (counter = 4095; counter > 0; counter--)
  {
    dac.setVoltage(counter, false);
  }
}