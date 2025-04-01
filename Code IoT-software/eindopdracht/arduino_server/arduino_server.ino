#include <Ethernet.h>

extern "C" {
#include "arduino_wrapper.h"
#include "buffermock.h"
#include "cserver.h"
extern unsigned long previousMillisYellow;

// Wrapper for httpClient.print
void printToClient(const char* message);
void setClient(EthernetClient* client);
}

// Function to check free RAM
extern int __heap_start, *__brkval;
int freeMemory() {
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start
                                  : (int)__brkval);
}

// CONSTANT FOR LEDS
const int YELLOW_LED_DELAY = 500;

// CONSTANT FOR DISTANCE SENSORS READINGS
const short SENSORS_DELAY = 100;

// TIMER FOR DISTANCE SENSORS
unsigned long previousSensorReading = 0;

// unique MAC address, correct IP address!
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 2, 3);
EthernetServer server(80);
EthernetClient httpClient;

CircularBuffer buffer1;
CircularBuffer buffer2;

// make httpClient methods available as ordinary functions
int clientAvailable() {
  return httpClient.connected() && httpClient.available();
}
char clientRead() { return httpClient.read(); }
char clientPeek() { return httpClient.peek(); }

void setup() {
  Serial.begin(9600);
  setupLeds();
  setupDistanceSensors();

  init_cserver(&buffer1, &buffer2);

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {

  httpClient = server.available();
  if (httpClient) {

    struct stream stream = {clientAvailable, clientPeek,
                            clientRead};

    handleRequest(stream);

    delay(1);
    httpClient.stop(); // close connection
  }

  // CHECK LAD STATES
  if (previousMillisYellow + YELLOW_LED_DELAY < millis() &&
      !allocationFailure) {
    digitalWrite(YELLOW, LOW);
  }

  // READ SENSORS
  if ((previousSensorReading + SENSORS_DELAY) < millis() &&
      activeMode) {
    updateSensorData();
    previousSensorReading = millis();
  }
}
