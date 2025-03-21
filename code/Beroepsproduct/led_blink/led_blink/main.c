/*
 * led_blink.c
 *
 * Created: 19/03/2025 14:35:32
 * Author : mikea
 */ 

#include <avr/io.h>


int main(void)
{
	unsigned long x;
	DDRB |= (1<<5);
	while(1){
		PORTB ^= (1<<5);
		for(x=0; x<200000; x++);
	}
}

