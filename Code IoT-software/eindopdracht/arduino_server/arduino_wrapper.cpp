// filepath: c:\Users\mikea\Documents\IoT-techniek_svn\Code IoT-software\eindopdracht\arduino_server\serial_wrapper.cpp
#include <Arduino.h>
#include "arduino_wrapper.h"

// const int GREEN = 5;
const int YELLOW = 4;
// const int RED1 = 9;
// const int RED2 = 8;

extern "C"
{
    unsigned long previousMillisYellow = 0;

    void setLed(int led, bool state)
    {
        digitalWrite(led, state);
    }

    void printToSerial(const char *message)
    {
        Serial.println(message);
    }

    unsigned long getMillis()
    {
        return millis();
    }
}