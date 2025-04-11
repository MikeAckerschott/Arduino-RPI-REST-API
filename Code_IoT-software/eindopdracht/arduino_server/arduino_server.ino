#include <Ethernet.h>

extern "C"
{
#include "arduino_wrapper.h"
#include "buffermock.h"
#include "cserver.h"
  extern unsigned long previousMillisYellow;
}

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
int clientAvailable()
{
  return httpClient.connected() && httpClient.available();
}
char clientRead() { return httpClient.read(); }
char clientPeek() { return httpClient.peek(); }

void setup()
{
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

void loop()
{

  // read digital pin 3
  //  Serial.println(digitalRead(interruptPin));

  httpClient = server.available();
  if (httpClient)
  {

    struct stream stream = {clientAvailable, clientPeek,
                            clientRead};

    handleRequest(stream);

    delay(1);
    httpClient.stop(); // close connection
  }

  // CHECK LED STATES
  if (previousMillisYellow + YELLOW_LED_DELAY < millis() &&
      !allocationFailure)
  {
    digitalWrite(YELLOW, LOW);
  }

  if (previousMillisGreen + GREEN_LED_DELAY < millis() &&
      !allocationFailure)
  {
    digitalWrite(GREEN, LOW);
  }

  // READ SENSORS
  if ((previousSensorReading + SENSORS_DELAY) < millis() &&
      activeMode)
  {
    updateSensorData();
    previousSensorReading = millis();
  }

  if (buffer1.count == buffer1.size)
  {
    digitalWrite(RED1, HIGH);
  }
  else
  {
    digitalWrite(RED1, LOW);
  }
  if (buffer2.count == buffer2.size)
  {
    digitalWrite(RED2, HIGH);
  }
  else
  {
    digitalWrite(RED2, LOW);
  }

  // once allocation failure occurs, all LEDs constantly on
  if (allocationFailure)
  {
    digitalWrite(GREEN, HIGH);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED1, HIGH);
    digitalWrite(RED2, HIGH);
  }
}

void onInterrupt()
{
  // reset langdurige gemiddeldes, leeg buffer en zet op
  // lengte 12
  if (!resize_buffer(&buffer1, 12) ||
      !resize_buffer(&buffer2, 12))
  {
    allocationFailure = true;
  }
  empty_buffer(&buffer1);
  empty_buffer(&buffer2);

  reset_aggregate(&aggregate1);
  reset_aggregate(&aggregate2);

  Serial.println("INTERRUPT RECEIVED");
  digitalWrite(GREEN, HIGH);
  previousMillisGreen = millis();

  // digitalWrite(GREEN, HIGH);
}
