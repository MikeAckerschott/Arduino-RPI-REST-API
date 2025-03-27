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

// AVERAGES AND STANDARD DEVIATIONS
unsigned long long totalReadings = 0;
unsigned long long totalSensor1 = 0;
unsigned long long totalSensor2 = 0;
unsigned long long totalSquaredSensor1 = 0;
unsigned long long totalSquaredSensor2 = 0;
double averageSensor1 = 0;
double averageSensor2 = 0;
double stdDevSensor1 = 0;
double stdDevSensor2 = 0;

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
    // READ SENSORS
    int sensor1 = 0;
    int sensor2 = 0;
    getDistances(&sensor1, &sensor2);
    Serial.print("sensors read: ");
    Serial.print(sensor1);
    Serial.print(" , ");
    Serial.print(sensor2);

    // INSERT INTO BUFFER
    insert_buffer(&buffer1, sensor1);
    insert_buffer(&buffer2, sensor2);

    // UPDATE RUNNING MEAN AND STANDARD DEVIATION WITH Welford's algorithm
    totalReadings++;
    totalSensor1 += sensor1;
    totalSensor2 += sensor2;
    totalSquaredSensor1 += sensor1 * sensor1;
    totalSquaredSensor2 += sensor2 * sensor2;
    averageSensor1 = totalSensor1 / static_cast<double>(totalReadings);
    averageSensor2 = totalSensor2 / static_cast<double>(totalReadings);

    stdDevSensor1 = sqrt((totalSquaredSensor1 - totalSensor1 * totalSensor1 / totalReadings) / static_cast<double>(totalReadings -1));
    stdDevSensor2 = sqrt((totalSquaredSensor2 - totalSensor2 * totalSensor2 / totalReadings) / static_cast<double>(totalReadings -1));

    Serial.print(" | average1: ");
    Serial.print(averageSensor1);
    Serial.print(" | average2: ");
    Serial.print(averageSensor2);
    Serial.print(" | stdDev1: ");
    Serial.print(stdDevSensor1);
    Serial.print(" | stdDev2: ");
    Serial.println(stdDevSensor2);

    previousSensorReading = millis();
  }
}
