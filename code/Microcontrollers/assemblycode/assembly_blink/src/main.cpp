#include <Arduino.h>

void setup() {
  DDRB |= (1 << PB5);
  while (1) {
      PINB = (1 << PB5);
  }
}
