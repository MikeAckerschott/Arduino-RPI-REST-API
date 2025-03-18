// Copied from https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial1-creating-freertos-task-to-blink-led-in-arduino-uno
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

const byte LEDPIN1 = 12;
const byte LEDPIN2 = 11;
const byte LEDPIN3 = 10;
const int DELAY1 = 200;
const int DELAY2 = 300;
const int DELAYPRINT = 1000;

const unsigned char bufferSize = 250;
char ptrTaskList[bufferSize];

void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);

void setup()
{
  pinMode(LEDPIN3, OUTPUT);
  xTaskCreate(
      TaskBlink1, "task1", 128, NULL, 1, NULL);

  xTaskCreate(
      TaskBlink2, "task2", 128, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop()
{
  while (true)
  {
    PORTB ^= (1 << 2);
  }
}

void TaskBlink1(void *pvParameters)
{
  pinMode(LEDPIN1, OUTPUT);
  while (1)
  {
    digitalWrite(LEDPIN1, HIGH);
    vTaskDelay(DELAY1 / portTICK_PERIOD_MS);
    digitalWrite(LEDPIN1, LOW);
    vTaskDelay(DELAY1 / portTICK_PERIOD_MS);
  }
}

void TaskBlink2(void *pvParameters)
{
  pinMode(LEDPIN2, OUTPUT);
  while (1)
  {
    digitalWrite(LEDPIN2, HIGH);
    vTaskDelay(DELAY2 / portTICK_PERIOD_MS);
    digitalWrite(LEDPIN2, LOW);
    vTaskDelay(DELAY2 / portTICK_PERIOD_MS);
  }
}
