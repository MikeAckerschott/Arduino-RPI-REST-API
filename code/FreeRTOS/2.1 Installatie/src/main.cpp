#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

// Task function prototype
void vTaskDoNothing(void *pvParameters);

void setup() {
  // Create the task
  xTaskCreate(
    vTaskDoNothing,    // Task function
    "Noth",            // Task name
    128,               // Stack size (in words, not bytes)
    NULL,              // Task parameters
    1,                 // Task priority
    NULL               // Task handle
  );
}

void loop() {
  // Empty loop as FreeRTOS handles tasks
}

void vTaskDoNothing(void *pvParameters) {
  (void) pvParameters; // Suppress unused parameter warning

  for (;;) {
    // Do nothing
    vTaskDelay(pdMS_TO_TICKS(10)); // Delay for 10 ms
  }
}