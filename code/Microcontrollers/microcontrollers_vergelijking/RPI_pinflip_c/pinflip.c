#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>

void pin_flip() {
    // Initialize the bcm2835 library
    if (!bcm2835_init()) {
        printf("bcm2835 initialization failed\n");
        exit(1);
    }

    printf("bcm2835 initialization succeeded\n");

    // Set the pin to output mode
    bcm2835_gpio_fsel(2, BCM2835_GPIO_FSEL_OUTP);

    printf("starting pinflip loop\n");
    while (1) {
        // Toggle the pin state (HIGH to LOW and vice versa)
        bcm2835_gpio_write(2, HIGH);  
        bcm2835_gpio_write(2, LOW);   
    }
    bcm2835_close();
}

int main() {
    pin_flip();
    return 0;
}
