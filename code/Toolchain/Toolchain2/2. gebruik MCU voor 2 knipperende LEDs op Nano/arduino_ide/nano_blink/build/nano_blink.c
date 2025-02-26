#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED1 PB0
#define LED2 PB1

// ISR for Timer1 compare match
ISR(TIMER1_COMPA_vect)
{
    asm volatile ("nop"); // Begin marker
    PORTB ^= (1 << LED1);
    PORTB ^= (1 << LED2);
    asm volatile ("nop"); // End marker
}

void setup_timer()
{
    // Set CTC mode (Clear Timer on Compare Match)
    TCCR1B |= (1 << WGM12);

    // Set compare value for 1Hz blink (assuming 16MHz clock and 1024 prescaler)
    OCR1A = 15624;

    // Enable Timer1 compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    // Set prescaler to 1024 and start the timer
    TCCR1B |= (1 << CS12) | (1 << CS10);
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
