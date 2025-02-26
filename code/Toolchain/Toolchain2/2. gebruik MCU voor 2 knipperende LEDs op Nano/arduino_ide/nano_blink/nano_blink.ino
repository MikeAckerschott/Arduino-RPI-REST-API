#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED1 PB0
#define LED2 PB1

// ISR for Timer1 compare match
ISR(TIMER1_COMPA_vect)
{
    asm volatile("nop"); // Begin marker
    PORTB ^= (1 << LED1);
    PORTB ^= (1 << LED2);
    asm volatile("nop"); // End marker
}

void setup_timer()
{
    // Set CTC mode (Clear Timer on Compare Match)  15.11.2 & table 15-5
    TCCR1B |= (1 << WGM12);

    // Set prescaler to 1024 and start the timer 15.11.2 - Table 15-6
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // 15.9.2
    // focna = fclock / (2 * prescaler * (1 + fOCnA))
    // prescaler = 1024
    // fOCnA = 1Hz
    // fclock = 16MHz

    // Set compare value for 1Hz used in CTC. When it reaches this value,
    // it will trigger an interrupt and reset the timer to 0. See 15.9.2
    OCR1A = 16000000 / (2 * 1024 * 1);

    // Enable Timer1 compare interrupt 15.7
    // Compare match A interrupt enable 15.11.8
    // Compare the counter value with the compare value in register A (OCR1A)
    TIMSK1 |= (1 << OCIE1A);
}

void setup_leds()
{
    DDRB |= (1 << LED1) | (1 << LED2);
    PORTB &= ~((1 << LED1) | (1 << LED2));
    PORTB ^= (1 << LED2);
}

int main()
{
    setup_leds();
    setup_timer();

    // Enable global interrupts
    sei();

    while (1)
    {
        // Main loop does nothing, all work is done in ISR
    }
}
