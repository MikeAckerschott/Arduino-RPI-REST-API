// filepath: c:\Users\mikea\Documents\IoT-techniek_svn\Code IoT-software\eindopdracht\arduino_server\serial_wrapper.cpp
#include <Arduino.h>
#include <Ethernet.h>
#include "arduino_wrapper.h"

const int GREEN = 5;
const int YELLOW = 4;
const int RED1 = 9;
const int RED2 = 8;

// Declare httpClient as extern
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

    void printToSerial(const char *message)
    {
        Serial.println(message);
    }

    void printToSerialInt(const int message)
    {
        Serial.println(message);
    }

    void printToSerialFloat(const float message)
    {
        Serial.println(message);
    }

    unsigned long getMillis()
    {
        return millis();
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

        // Wrapper for httpClient.print
    void printToClientFloat(const float message)
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