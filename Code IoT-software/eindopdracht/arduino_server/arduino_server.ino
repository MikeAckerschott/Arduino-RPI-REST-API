#include <Ethernet.h>

extern "C"
{
#include "cserver.h"
#include "buffermock.h"
  extern unsigned long previousMillisYellow;
}

// CONSTANT FOR LEDS
const int GREEN = 5;
const int YELLOW = 4;
const int RED1 = 9;
const int RED2 = 8;
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
int clientAvailable()
{
  return httpClient.connected() && httpClient.available();
}
char clientRead()
{
  return httpClient.read();
}
char clientPeek()
{
  return httpClient.peek();
}

void setup()
{
  Serial.begin(9600);
  setupLeds();
  setupDistanceSensors();

  init_buffer(&buffer1, 12);
  init_buffer(&buffer2, 12);

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop()
{
  httpClient = server.available();
  if (httpClient)
  {
    Serial.println("new client");

    struct stream stream = {clientAvailable, clientPeek,
                            clientRead};
    struct response resp = handleRequest(stream);

    delay(1);
    httpClient.stop(); // close connection
    Serial.println("client disconnected");
  }

  // CHECK LAD STATES
  if (previousMillisYellow + YELLOW_LED_DELAY < millis())
  {
    digitalWrite(YELLOW, LOW);
  }

  // READ SENSORS
  if (previousSensorReading + SENSORS_DELAY < millis())
  {
    updateSensorData();
  }
}
