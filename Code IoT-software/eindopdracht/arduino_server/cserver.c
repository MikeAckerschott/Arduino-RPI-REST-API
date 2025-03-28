#include "cserver.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "abnf.h"
#include "arduino_wrapper.h"
#include "request.h"

extern void printToSerial(const char *message);
extern unsigned long getMillis();
extern void setLed(int led, bool state);

extern unsigned long previousMillisYellow;

// PUT /config/mode active of passive in msg body
// PUT /config/cbuffsize int waarde in msg body
// DELETE /sensors/1 verwijder alle waarden voor 1|2
// POST /sensors/1 ontvang int meetwaarde voor 1|2
// GET /sensors/1/avg geef μ voor 1|2 als in 38.2
// GET /sensors/2/stdev geef σ voor 1|2 als in 5.3
// GET /sensors/1/actual geef m uit buffer als in 37.3

const char *ALLOWED_ENDPOINTS[] =
    {
        "/config/mode",
        "/config/cbuffsize",
        "/sensors/1",
        "/sensors/2",
        "/sensors/1/avg",
        "/sensors/2/avg",
        "/sensors/1/stdev",
        "/sensors/2/stdev",
        "/sensors/1/actual",
        "/sensors/2/actual"};

struct AllowedCall
{
  const char *call;
  void (*handler)(const char *body);
};

void handlePutConfigMode(const char *body)
{
  printToSerial("Handling PUT /config/mode");
  // Add logic to handle this call
}

void handlePutConfigCbuffsize(const char *body)
{
  printToSerial("Handling PUT /config/cbuffsize");
  // Add logic to handle this call
}

void handleDeleteSensors1(const char *body)
{
  printToSerial("Handling DELETE /sensors/1");
  // Add logic to handle this call
}

void handleDeleteSensors2(const char *body)
{
  printToSerial("Handling DELETE /sensors/2");
  // Add logic to handle this call
}

void handlePostSensors1(const char *body)
{
  printToSerial("Handling POST /sensors/1");
  // Add logic to handle this call
}

void handlePostSensors2(const char *body)
{
  printToSerial("Handling POST /sensors/2");
  // Add logic to handle this call
}

void handleGetSensors1Avg(const char *body)
{
  printToSerial("Handling GET /sensors/1/avg");
  // Add logic to handle this call
}

void handleGetSensors2Avg(const char *body)
{
  printToSerial("Handling GET /sensors/2/avg");
  // Add logic to handle this call
}

void handleGetSensors1Stdev(const char *body)
{
  printToSerial("Handling GET /sensors/1/stdev");
  // Add logic to handle this call
}

void handleGetSensors2Stdev(const char *body)
{
  printToSerial("Handling GET /sensors/2/stdev");
  // Add logic to handle this call
}

void handleGetSensors1Actual(const char *body)
{
  printToSerial("Handling GET /sensors/1/actual");
  // Add logic to handle this call
}

void handleGetSensors2Actual(const char *body)
{
  printToSerial("Handling GET /sensors/2/actual");
  // Add logic to handle this call
}

struct AllowedCall ALLOWED_CALLS[] =
    {
        {"PUT /config/mode", handlePutConfigMode},
        {"PUT /config/cbuffsize", handlePutConfigCbuffsize},
        {"DELETE /sensors/1", handleDeleteSensors1},
        {"DELETE /sensors/2", handleDeleteSensors2},
        {"POST /sensors/1", handlePostSensors1},
        {"POST /sensors/2", handlePostSensors2},
        {"GET /sensors/1/avg", handleGetSensors1Avg},
        {"GET /sensors/2/avg", handleGetSensors2Avg},
        {"GET /sensors/1/stdev", handleGetSensors1Stdev},
        {"GET /sensors/2/stdev", handleGetSensors2Stdev},
        {"GET /sensors/1/actual", handleGetSensors1Actual},
        {"GET /sensors/2/actual", handleGetSensors2Actual}};

void handleRequest(struct stream stream)
{
  // response received from the client. yellow led on for 0.5 seconds
  setLed(YELLOW, true);
  previousMillisYellow = getMillis();

  char buffer[256] = {0};
  int bufferIndex = 0;

  while (stream.available() > 0 && bufferIndex < sizeof(buffer) - 1)
  {
    buffer[bufferIndex++] = stream.read();
  }
  buffer[bufferIndex] = '\0';

  // check for the line that contains Content-Length:
  char *contentLengthLine = strstr(buffer, "Content-Length:");
  int contentLength = 0;
  char *body = NULL;
  if (contentLengthLine != NULL)
  {
    // get the content length value
    char *contentLengthValue = contentLengthLine + strlen("Content-Length: ");
    contentLength = atoi(contentLengthValue);
    printToSerialInt(contentLength);

    // get the last line (body) method (first word) and target (second word)
    char *lastLine = strrchr(buffer, '\n');
    body = lastLine + 1;
    int bodyLength = strlen(body);
    printToSerial(body);
  }

  // get the first line (PUT /config/mode HTTP/1.1) and remove the http version
  char *firstLine = strtok(buffer, "\n");
  char *method = strtok(firstLine, " ");
  char *target = strtok(NULL, " ");

  printToSerial(method);
  printToSerial(target);

  // check if target is in ALLOWED_ENDPOINTS
  bool targetAllowed = false;
  for (int i = 0; i < sizeof(ALLOWED_ENDPOINTS) / sizeof(ALLOWED_ENDPOINTS[0]); i++)
  {
    if (strcmp(target, ALLOWED_ENDPOINTS[i]) == 0)
    {
      printToSerial("Target allowed");
      targetAllowed = true;
      break;
    }
  }
  if (!targetAllowed)
  {
    printToSerial("Target not allowed");
    char *fullCall = (char *)malloc(strlen(method) + strlen(target) + 2); // +2 for space and null terminator
  }

  // check if method is in ALLOWED_CALLS
  char *fullCall = (char *)malloc(strlen(method) + strlen(target) + 2); // +2 for space and null terminator
  if (fullCall == NULL)
  {
    printToSerial("Memory allocation failed");
    return;
  }
  strcpy(fullCall, method);
  strcat(fullCall, " ");
  strcat(fullCall, target);

  bool methodAllowed = false;
  for (int iterator = 0; iterator < sizeof(ALLOWED_CALLS) / sizeof(ALLOWED_CALLS[0]); iterator++)
  {
    if (strcmp(fullCall, ALLOWED_CALLS[iterator].call) == 0)
    {
      printToSerial(ALLOWED_CALLS[iterator].call);
      printToSerial(fullCall);
      printToSerial("Method allowed");
      methodAllowed = true;
      // Call the corresponding handler function
      ALLOWED_CALLS[iterator].handler(body);
      free(fullCall);
      return;
    }
  }

  if (!methodAllowed)
  {
    printToSerial("Method not allowed");
  }
  free(fullCall);
}

void setBuffer1(CircularBuffer *buffer)
{
  buffer_1 = buffer;
}
void setBuffer2(CircularBuffer *buffer)
{
  buffer_2 = buffer;
}
