#include <Arduino.h>

void setup() {
  pinMode(5, OUTPUT);
  while(1){
    WRITE_PERI_REG( 0x60000304, 0x20 );
    WRITE_PERI_REG( 0x60000308, 0x20 );
  }
}

void loop() {}
