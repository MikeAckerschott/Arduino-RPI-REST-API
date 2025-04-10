#include <Arduino.h>
#include <Ethernet.h>
#include "arduino_wrapper.h"

const int GREEN = 5;
const int YELLOW = 4;
const int RED1 = 9;
const int RED2 = 8;

extern EthernetClient httpClient;

extern "C"
{
    unsigned long previousMillisYellow = 0;

    void setLed(int led, bool state, bool allocationFailure)
    {
        if(allocationFailure){
            return;
        }
        digitalWrite(led, state);
    }

    unsigned long getMillis()
    {
        return millis();
    }

    void printToSerial(const char *message)
    {
        Serial.println(message);
    }

    void printToSerialInline(const char *message)
    {
        Serial.print(message);
    }

    void printToSerialInt(const int message)
    {
        Serial.println(message);
    }

    // Wrapper for httpClient.print
    void printToClient(const char *message)
    {
        if (httpClient)
        {
            if (httpClient.connected())
            {
                httpClient.print(message);
            }
            else
            {
                Serial.println("Client not connected");
            }
        }
    }

    void printToClientInt(const int message)
    {
        if (httpClient)
        {
            if (httpClient.connected())
            {
                httpClient.print(message);
            }
            else
            {
                Serial.println("Client not connected");
            }
        }
    }
}