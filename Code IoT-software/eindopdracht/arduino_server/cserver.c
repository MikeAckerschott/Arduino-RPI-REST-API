#include "cserver.h"
#include "abnf.h"
#include "arduino_wrapper.h"

extern void printToSerial(const char *message);
extern unsigned long getMillis();
extern void setLed(int led, bool state);

extern unsigned long previousMillisYellow;

struct response handleRequest(struct stream stream)
{
  //response received from the client. yellow led on for 0.5 seconds
  setLed(YELLOW, true);
  previousMillisYellow = getMillis();

  struct response response;
  char buffer[128] = {0};
  int bufferIndex = 0;

  while (stream.available() > 0 && bufferIndex < sizeof(buffer) - 1)
  {
    buffer[bufferIndex++] = stream.read();
  }
  buffer[bufferIndex] = '\0';

  // Print the buffer to the Serial Monitor
  printToSerial("Buffer content:");
  printToSerial(buffer);

  /*
  23:29:08.368 -> GET /sensors/1/avg HTTP/1.1
  23:29:08.402 -> Host: 192.168.2.3
  23:29:08.434 -> User-Agent: curl/7.81.0
  23:29:08.467 -> Accept:

  Dont care about Host, User-agent or accept
  find the endpoint called and please client

  */

  return response;
}
