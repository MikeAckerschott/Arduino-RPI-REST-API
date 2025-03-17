#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

// Task function prototype
void vTaskDoNothing1(void *pvParameters);
void vTaskDoNothing2(void *pvParameters);

void setup()
{
  // Create the task
  xTaskCreate(
      vTaskDoNothing1, // Task function
      "Noth1",        // Task name
      500,            // Stack size (in words, not bytes)
      NULL,           // Task parameters
      1,              // Task priority
      NULL            // Task handle
  );
  // xTaskCreate(
  //     vTaskDoNothing2, // Task function
  //     "Noth2",        // Task name
  //     650,            // Stack size (in words, not bytes)
  //     NULL,           // Task parameters
  //     1,              // Task priority
  //     NULL            // Task handle
  // );
}

void loop()
{
  // Empty loop as FreeRTOS handles tasks
}

void vTaskDoNothing1(void *pvParameters)
{
  (void)pvParameters; // Suppress unused parameter warning

  for (;;)
  {
    // Do nothing
    vTaskDelay(pdMS_TO_TICKS(10)); // Delay for 10 ms
  }
}

void vTaskDoNothing2(void *pvParameters)
{
  (void)pvParameters; // Suppress unused parameter warning

  for (;;)
  {
    // Do nothing
    vTaskDelay(pdMS_TO_TICKS(10)); // Delay for 10 ms
  }
}