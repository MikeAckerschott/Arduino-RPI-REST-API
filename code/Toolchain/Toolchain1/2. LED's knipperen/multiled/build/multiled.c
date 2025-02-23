#include <avr/interrupt.h>

#define MS_DELAY_PORTB5 500
#define MS_DELAY_PORTB4 1000

uint8_t timer_flag_PORTB5 = 0;
uint8_t timer_flag_PORTB4 = 0;

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
    static uint16_t count_PORTB5 = 0;
    static uint16_t count_PORTB4 = 0;
    handle_timer(&count_PORTB5, &timer_flag_PORTB5, MS_DELAY_PORTB5);
    handle_timer(&count_PORTB4, &timer_flag_PORTB4, MS_DELAY_PORTB4);
}

int main(void) {
    DDRB |= _BV(DDB5);
    DDRB |= _BV(DDB4);
    timer0_init();

    while (1) {
        if (timer_flag_PORTB5) {
            PORTB ^= _BV(PORTB5);
            timer_flag_PORTB5 = 0;
        }
        if (timer_flag_PORTB4) {
            PORTB ^= _BV(PORTB4);
            timer_flag_PORTB4 = 0;
        }
    }
}
