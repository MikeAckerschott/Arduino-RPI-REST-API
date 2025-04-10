#include <Ethernet.h>

extern "C" {
#include "arduino_wrapper.h"
#include "buffermock.h"
#include "cserver.h"
extern unsigned long previousMillisYellow;
}

// Function to calculate free memory
#ifdef __arm__
// for ARM architecture
extern "C" char* sbrk(int incr);
int freeMemory() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#else
// for AVR architecture
extern unsigned int __heap_start, *__brkval;
int freeMemory() {
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start
                                  : (int)__brkval);
}
#endif

// CONSTANT FOR LEDS
const int YELLOW_LED_DELAY = 500;
const int GREEN_LED_DELAY = 500;

unsigned long previousMillisGreen = 0;

// CONSTANT FOR DISTANCE SENSORS READINGS
const short SENSORS_DELAY = 100;

// TIMER FOR DISTANCE SENSORS
unsigned long previousSensorReading = 0;

// INTERRUPT PIN UNO
const byte interruptPin = 3;

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

  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin),
                  onInterrupt, RISING);
  init_cserver(&buffer1, &buffer2);

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // Serial.println("loop");

  // read digital pin 3
  //  Serial.println(digitalRead(interruptPin));
  if (allocationFailure) {
    digitalWrite(GREEN, HIGH);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED1, HIGH);
    digitalWrite(RED2, HIGH);
  }

  httpClient = server.available();
  if (httpClient) {

    struct stream stream = {clientAvailable, clientPeek,
                            clientRead};

    handleRequest(stream);
    clearRequest();

    delay(1);
    httpClient.stop(); // close connection
  }

  // CHECK LED STATES
  if (previousMillisYellow + YELLOW_LED_DELAY < millis() &&
      !allocationFailure) {
    digitalWrite(YELLOW, LOW);
  }

  if (previousMillisGreen + GREEN_LED_DELAY < millis() &&
      !allocationFailure) {
    digitalWrite(GREEN, LOW);
  }

  // READ SENSORS
  if ((previousSensorReading + SENSORS_DELAY) < millis() &&
      activeMode) {
    updateSensorData();
    previousSensorReading = millis();
  }
}

void onInterrupt() {
  // reset langdurige gemiddeldes, leeg buffer en zet op
  // lengte 12
  if (!resize_buffer(&buffer1, 12) ||
      !resize_buffer(&buffer2, 12)) {
    allocationFailure = true;
  }
  empty_buffer(&buffer1);
  empty_buffer(&buffer2);

  reset_aggregate(&aggregate1);
  reset_aggregate(&aggregate2);

  Serial.println("INTERRUPT RECEIVED");
  digitalWrite(GREEN, HIGH);
  previousMillisGreen = millis();

  // print leftover dynamic memory

  Serial.print("Free memory: ");
  Serial.println(freeMemory());

  // digitalWrite(GREEN, HIGH);
}
