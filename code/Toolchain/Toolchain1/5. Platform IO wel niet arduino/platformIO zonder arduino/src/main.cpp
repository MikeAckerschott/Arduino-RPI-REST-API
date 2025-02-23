#include <Arduino.h>
#include <avr/interrupt.h>

#define MS_DELAY_1HZ 500
#define MS_DELAY_10HZ 50

volatile uint8_t timer_flag = 0;
uint16_t blink_delay = MS_DELAY_1HZ;

void timer0_init() {
    TCCR0A |= (1 << WGM01);
    OCR0A = 249;
    TIMSK0 |= (1 << OCIE0A);
    TCCR0B |= (1 << CS01) | (1 << CS00);
    sei();
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
    DDRB |= (1 << DDB5) | (1 << DDB4);
    DDRD &= ~(1 << DDD4);
    PORTD |= (1 << PORTD4);
}

int main(void) {
    setup_pins();
    timer0_init();

    while (1) {
        //check button pressed
        if (!(PIND & (1 << PIND4))) {
            blink_delay = MS_DELAY_10HZ;
        } else {
            blink_delay = MS_DELAY_1HZ;
        }

        //blink
        if (timer_flag) {
            PORTB ^= (1 << PORTB5);
            PORTB ^= (1 << PORTB4);
            timer_flag = 0;
        }
    }
}
