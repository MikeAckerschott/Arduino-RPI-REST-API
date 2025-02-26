#include <avr/io.h>
#include <util/delay.h>

#define MS_DELAY 500

int main (void) {
    //page 13.2.3 
    DDRB |= _BV(DDB5);

    while(1) {
        //13.2.2 Toggling a pin
        PORTB |= _BV(PORTB5);
        _delay_ms(MS_DELAY);
        PORTB &= ~_BV(PORTB5);
        _delay_ms(MS_DELAY);
    }
}
