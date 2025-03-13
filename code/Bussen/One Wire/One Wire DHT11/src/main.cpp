#include <DHT11.h>
#include <Arduino.h>

DHT11 dht11(2);

void setup() {
    dht11.setDelay(140); 
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
}

void loop() {
    PORTB ^= (1 << 5);
    PORTB ^= (1 << 5);

    int temperature = 0;
    int humidity = 0;

    int result = dht11.readTemperatureHumidity(temperature, humidity);

    while (result != 0) {
        result = dht11.readTemperatureHumidity(temperature, humidity);
    }
}
