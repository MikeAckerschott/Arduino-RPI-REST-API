#include <Arduino.h>

void setup()
{
  pinMode(13, OUTPUT);
  noInterrupts(); 

  while(1){
    PORTB = B00100000;
    PORTB = B00000000;
    PORTB = B00100000;
    PORTB = B00000000;
    PORTB = B00100000;
    PORTB = B00000000;
    PORTB = B00100000;
    PORTB = B00000000;
    PORTB = B00100000;
    PORTB = B00000000;
    PORTB = B00100000;
    PORTB = B00000000;
    PORTB = B00100000;
    PORTB = B00000000;
    PORTB = B00100000;
    PORTB = B00000000;
  }
}

void loop()
{
}
