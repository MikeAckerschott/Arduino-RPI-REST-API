#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

  Serial.println(uxTaskGetNumberOfTasks());
}

void loop()
{
}
