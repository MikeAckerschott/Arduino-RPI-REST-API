#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t counter = 0;

int main(void)
{ 
  cli(); // Disable global interrupts
  TCCR0B |= ((1 << CS02) | (1 << CS00)); // Timer 0 prescaling - divides by 1024
  TCCR0A |= (1 << WGM01); // Put timer 0 in CTC mode
  OCR0A = 80; // Count 80 cycles for interrupt at 8MHz gives 10ms
  TIMSK |= (1 << OCIE0A); // Enable timer compare interrupt
  DDRB |= (1 << PB0); // Set PortB Pin0 as an output
  DDRB |= (1 << PB1); // Set PortB Pin1 as an output
  PORTB |= (1 << PB0); // Set PortB Pin0 high to turn on LED
  sei(); // Enable global interrupts
  while(1) { } // Don't do anything in main
}

ISR(TIMER0_COMPA_vect) // Interrupt Service Routine
{ 
  counter++;
  if (counter >= 5) { // 5 * 10ms = 50ms
    PORTB ^= (1 << PB0); // Use xor to toggle the LED
    PORTB ^= (1 << PB1); // Use xor to toggle the LED
    counter = 0;
  }
}