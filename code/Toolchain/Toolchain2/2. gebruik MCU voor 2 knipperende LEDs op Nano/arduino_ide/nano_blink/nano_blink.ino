#include <avr/io.h>
#include <avr/interrupt.h>

#define LED1 PB0
#define LED2 PB1
volatile uint8_t led_status = 0;

// ISR voor externe interrupt INT0
ISR(INT0_vect) {
    asm volatile ("nop"); // Begin marker
    
    if (led_status) {
        PORTB &= ~(1 << LED1);
        PORTB |= (1 << LED2);
    } else {
        PORTB &= ~(1 << LED2);
        PORTB |= (1 << LED1);
    }
    
    led_status = !led_status;
    
    asm volatile ("nop");  // Einde marker
}

void setup_interrupt() {
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);  
    
    // Zet interrupt op rising edge (ISC01 = 1, ISC00 = 1)
    EICRA |= (1 << ISC01) | (1 << ISC00);
    
    // Schakel externe interrupt INT0 in
    EIMSK |= (1 << INT0);
    
    // Globale interrupts inschakelen
    sei();
}

void setup_leds() {
    DDRB |= (1 << LED1) | (1 << LED2);
    PORTB &= ~((1 << LED1) | (1 << LED2));
}

int main() {
    setup_leds();
    setup_interrupt();

    while (1) {
    }
}
