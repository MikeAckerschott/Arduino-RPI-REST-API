/*
 * alternating_leds_button_interrupt.c
 *
 * Created: 19/03/2025 14:43:09
 * Author : mikea
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define LED1 PB5
#define LED2 PB4
#define TOGGLE PB3

// Define what should happen when Low to High transition is detected
ISR(INT0_vect)
{
	PORTB ^= (1 << LED1); // Toggle LED1
	PORTB ^= (1 << LED2); // Toggle LED2
	
}

void setup_interrupt()
{
	DDRD &= ~(1 << PD2); // Set pin 2 as input. Datasheet 13.2.1 for input/output. 13.4.9 for registers
	PORTD |= (1 << PD2); // Enable internal pull-up resistor on pin 2

	// Pin Low to High triggers an interrupt. 12.2.1
	EICRA |= (1 << ISC01) | (1 << ISC00);

	// Enable external interrupts. 12.2.2
	EIMSK |= (1 << INT0);

	// Enable global interrupts
	sei();
}

void setup_leds()
{
	DDRB |= (1 << LED1) | (1 << LED2) | (1 << TOGGLE);     // Set LED pins as output
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
		PORTB ^= (1 << TOGGLE); 
	}
}


