#include <avr/interrupt.h>

#define MS_DELAY_PORTB5 1000
#define MS_DELAY_PORTB4 2000

uint8_t timer_flag_PORTB5 = 0;
uint8_t timer_flag_PORTB4 = 0;

//calculating this value is explained in the datasheet 14.7.2
int8_t calculateOCR0A(uint32_t clockspeed, uint32_t prescaler, uint32_t frequency) {
    return clockspeed / (prescaler * frequency) - 1;
}

void timer0_init() {
    // CTC on timer 0 - datasheet 14.7.2
    TCCR0A |= (1 << WGM01);

    // Set prescaler to 1024. datasheet table 14-9
    TCCR0B |= (1 << CS02) | (1 << CS00);

    // Trigger interrupt once every MS
    OCR0A = calculateOCR0A(16000000, 1024, 1000);

    // Enable Timer0 compare interrupt. datasheet 14.9.6
    TIMSK0 |= (1 << OCIE0A);

    // Enable global interrupts
    sei();
}

void handle_timer(uint16_t *count, uint8_t *flag, uint16_t delay) {
    (*count)++;
    if (*count >= delay) {
        *flag = 1;
        *count = 0;
    }
}

// calls an interrupt when TCNT0 is equal or greater than OCR0A and resets TCNT0 (CTC mode)
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
