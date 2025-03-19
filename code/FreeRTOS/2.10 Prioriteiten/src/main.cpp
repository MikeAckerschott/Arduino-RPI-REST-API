// Copied from https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial1-creating-freertos-task-to-blink-led-in-arduino-uno
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

const byte LEDPIN1 = 12;
const byte LEDPIN2 = 11;
const byte LEDPIN3 = 10;
const int DELAY1 = 200;
const int DELAY2 = 300;

const unsigned char bufferSize = 250;
char ptrTaskList[bufferSize];

void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void TaskBlink3(void *pvParameters);

void setup()
{
  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  pinMode(LEDPIN3, OUTPUT);
  xTaskCreate(
      TaskBlink1, "task1", 128, NULL, 2, NULL);

  xTaskCreate(
      TaskBlink2, "task2", 128, NULL, 2, NULL);

  xTaskCreate(
      TaskBlink3, "task3", 128, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop()
{
}

void TaskBlink1(void *pvParameters)
{
  while (1)
  {
    digitalWrite(LEDPIN1, HIGH);
    delay(DELAY1 / portTICK_PERIOD_MS);
    digitalWrite(LEDPIN1, LOW);
    delay(DELAY1 / portTICK_PERIOD_MS);
  }
}

void TaskBlink2(void *pvParameters)
{
  while (1)
  {
    digitalWrite(LEDPIN2, HIGH);
    delay(DELAY2 / portTICK_PERIOD_MS);
    digitalWrite(LEDPIN2, LOW);
    delay(DELAY2 / portTICK_PERIOD_MS);
  }
}

// toggle task based on 2.4
void TaskBlink3(void *pvParameters)
{
  while (1)
  {
    PORTB |= (1 << PORTB2);
    PORTB &= ~(1 << PORTB2);
  }
}