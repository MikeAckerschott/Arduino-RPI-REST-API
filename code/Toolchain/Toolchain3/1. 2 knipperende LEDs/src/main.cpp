#include <avr/interrupt.h>

#define MS_DELAY_1HZ 500
#define MS_DELAY_10HZ 50

volatile uint8_t timer_flag = 0;
uint16_t blink_delay = MS_DELAY_1HZ;

void timer0_init() {
  TCCR0A |= (1 << WGM01); // Set CTC mode
  OCR0A = 124; // Set compare value for 1ms interrupt
  TIMSK |= (1 << OCIE0A); // Enable compare match interrupt
  TCCR0B |= (1 << CS01) | (1 << CS00); // Set prescaler to 64 and start timer
  sei(); // Enable global interrupts
}

void handle_timer(uint16_t *count, uint8_t *flag, uint16_t delay) {
  (*count)++;
  if (*count >= delay) {
    *flag = 1;
    *count = 0;
  }
}

ISR(TIMER0_COMPA_vect) {
  static uint16_t count = 0;
  handle_timer(&count, &timer_flag, blink_delay);
}

void setup_pins() {
  DDRB |= (1 << DDB0) | (1 << DDB1); // Set PB0 and PB1 as output for LEDs
  DDRB &= ~(1 << DDB2); // Set PB2 as input for button
  PORTB |= (1 << PORTB2); // Enable pull-up resistor on PB2
  PORTB ^= (1 << PORTB0); // Toggle LED on PB0
}

int main(void) {
  setup_pins();
  timer0_init();

  while (1) {
    // Check if button is pressed
    if (!(PINB & (1 << PINB2))) {
      blink_delay = MS_DELAY_10HZ;
    } else {
      blink_delay = MS_DELAY_1HZ;
    }

    // Blink LEDs
    if (timer_flag) {
      PORTB ^= (1 << PORTB0); // Toggle LED on PB0
      PORTB ^= (1 << PORTB1); // Toggle LED on PB1
      timer_flag = 0;
    }
  }
}
