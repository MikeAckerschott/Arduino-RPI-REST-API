// filepath: c:\Users\mikea\Documents\IoT-techniek_svn\Code IoT-software\eindopdracht\arduino_server\serial_wrapper.cpp
#include <Arduino.h>

extern "C" void printToSerial(const char* message) {
    Serial.println(message);
}