#include <Arduino.h>

enum LEDState {
    OFF,
    SLOW_BLINK,
    FAST_BLINK,
    SHORT_ON_LONG_OFF,
    LONG_ON_SHORT_OFF,
    EVEN_BLINK
};

struct LED {
    uint8_t pin;
    LEDState state;
    unsigned long previousMillis;
    unsigned long interval;
    bool ledState;
};

LED leds[] = {
    {13, SLOW_BLINK, 0, 1000, false},
    {12, FAST_BLINK, 0, 200, false},
    {11, SHORT_ON_LONG_OFF, 0, 200, false},
    {10, LONG_ON_SHORT_OFF, 0, 200, false},
    {9, EVEN_BLINK, 0, 200, false},
    {8, OFF, 0, 200, false},
};

void updateLED(LED &led) {
    unsigned long currentMillis = millis();
    switch (led.state) {
        case OFF:
            digitalWrite(led.pin, LOW);
            break;
        case SLOW_BLINK:
            led.interval = 1000;
            break;
        case FAST_BLINK:
            led.interval = 200;
            break;
        case SHORT_ON_LONG_OFF:
            led.interval = led.ledState ? 200 : 1800;
            break;
        case LONG_ON_SHORT_OFF:
            led.interval = led.ledState ? 1800 : 200;
            break;
        case EVEN_BLINK:
            led.interval = 500;
            break;
    }
    if (led.state != OFF && currentMillis - led.previousMillis >= led.interval) {
        led.previousMillis = currentMillis;
        led.ledState = !led.ledState;
        digitalWrite(led.pin, led.ledState ? HIGH : LOW);
    }
}

void setup() {
    for (auto &led : leds) {
        pinMode(led.pin, OUTPUT);
        digitalWrite(led.pin, LOW);
    }
}

void loop() {
    for (auto &led : leds) {
        updateLED(led);
    }
}
