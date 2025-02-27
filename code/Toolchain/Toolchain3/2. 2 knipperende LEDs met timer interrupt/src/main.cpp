#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t counter = 0;

// datasheet 11.7.2
int8_t calculateOCR0A(uint32_t clockspeed, uint32_t prescaler, uint32_t frequency) {
  return clockspeed / (prescaler * frequency) - 1;
}


int main(void)
{ 
  cli(); // Disable global interrupts

  TCCR0A |= (1 << WGM01); // Put timer 0 in CTC mode. 11.7.2 Datasheet

  OCR0A = calculateOCR0A(8000000, 1024, 10000); // Set compare value for 10ms interrupt

  TIMSK |= (1 << OCIE0A); // Enable compare match interrupt. 11.9.7 Data sheet

  TCCR0B |= ((1 << CS02) | (1 << CS00)); // Timer 0 prescaler to 1024. Table 11-6 Datasheet
  
  DDRB |= (1 << PB0); // Set PortB Pin0 as an output
  DDRB |= (1 << PB1); // Set PortB Pin1 as an output
  PORTB |= (1 << PB0); // Set PortB Pin0 high to turn on LED
  sei(); // Enable global interrupts
  while(1) { } // Don't do anything in main
}

ISR(TIMER0_COMPA_vect) // Interrupt Service Routine
{ 
    // Use xor to toggle the LED
    PORTB ^= (1 << PB0); 
    PORTB ^= (1 << PB1); 
}