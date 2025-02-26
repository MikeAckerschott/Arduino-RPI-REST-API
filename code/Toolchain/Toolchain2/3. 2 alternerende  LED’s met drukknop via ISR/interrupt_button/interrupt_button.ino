#include <avr/io.h>
#include <avr/interrupt.h>

#define LED1 PB0
#define LED2 PB1

ISR(INT0_vect)
{
    //Only PD2 is configured as input, so no need to check which pin triggered the interrupt
    PORTB ^= (1 << LED1); // Toggle LED1
    PORTB ^= (1 << LED2); // Toggle LED2
}

void setup_interrupt()
{ //DDRD &= 11111011
    DDRD &= ~(1 << PD2); // Set PD2 as input

    // Pin Low to High triggers an interrupt. 12.2.1
    EICRA |= (1 << ISC01) | (1 << ISC00);

    // Enable external interrupts. 12.2.2
    EIMSK |= (1 << INT0);

    // Enable global interrupts
    sei();
}

void setup_leds()
{
    DDRB |= (1 << LED1) | (1 << LED2);     // Set LED pins as output
    PORTB &= ~((1 << LED1) | (1 << LED2)); // Turn off both LEDs initially
    PORTB ^= (1 << LED2); // Toggle LED2
}

int main()
{
    setup_leds();
    setup_interrupt();

    while (1)
    {
        // Main loop does nothing, all work is done in ISR
    }
}
