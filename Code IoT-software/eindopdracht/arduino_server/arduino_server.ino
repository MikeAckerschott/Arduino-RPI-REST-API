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
  // give cserver access to the buffers
  setBuffer1(&buffer1);
  setBuffer2(&buffer2);

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
    Serial.println(resp.code);
    // send response
    switch (resp.code)
    {
    case INTERNAL_SERVER_ERROR_500:
      httpClient.println("HTTP/1.1 500 Internal Server Error");
      Serial.println("failed to malloc cbuffers");
      break;
    case BAD_REQUEST_400:
      httpClient.println("HTTP/1.1 400 Bad Request");
      Serial.println("bad request");
      break;
    case NOT_FOUND_404:
      httpClient.println("HTTP/1.1 404 Not Found");
      Serial.println("request target not found");
      break;
    case OK_200_GET_AVG:
      httpClient.println("HTTP/1.1 200 OK");
      break;
    case OK_200_GET_STDEV:
      httpClient.println("HTTP/1.1 200 OK");
      break;
    case OK_200_GET_ACTUAL:
      httpClient.println("HTTP/1.1 200 OK");
      break;
    case CREATED_201_PUT_MODE_ACTIVE:
      httpClient.println("HTTP/1.1 201 Created");
      Serial.println("starting sensor readings");
      break;
    case CREATED_201_PUT_MODE_PASSIVE:
      httpClient.println("HTTP/1.1 201 Created");
      Serial.println("stopping sensor readings");
      break;
    case CREATED_201_PUT_CBUFFSIZE:
      httpClient.println("HTTP/1.1 201 Created");
      break;
    case CREATED_201_POST_MEASUREMENT:
      httpClient.println("HTTP/1.1 201 Created");
      break;
    case CREATED_201_DELETE_MEASUREMENTS:
      httpClient.println("HTTP/1.1 201 Created");
      break;
    default:
      httpClient.println("HTTP/1.1 500 Internal Server Error");
      Serial.println("unknown response code");
      break;
    }

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
