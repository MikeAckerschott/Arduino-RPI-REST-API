#include <avr/interrupt.h>

#define MS_DELAY_1HZ 500
#define MS_DELAY_10HZ 50

volatile uint8_t timer_flag = 0;
uint16_t blink_delay = MS_DELAY_1HZ;

// calculating this value is explained in the datasheet 14.7.2
int8_t calculateOCR0A(uint32_t clockspeed, uint32_t prescaler, uint32_t frequency) {
    return clockspeed / (prescaler * frequency) - 1;
}

void timer0_init() {
    // CTC on timer 0. datasheet 14.7.2
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
    static uint16_t count = 0;
    handle_timer(&count, &timer_flag, blink_delay);
}

void setup_pins() {
    DDRB |= (1 << DDB5) | (1 << DDB4);
    DDRD &= ~(1 << DDD4);
    // PORTD |= (1 << PORTD4);
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
