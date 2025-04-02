#include "cserver.h"
#include "arduino_wrapper.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void printToSerial(const char* message);
extern void printToSerialInt(const int message);
extern void printToSerialFloat(const float message);
extern void printToClient(const char* message);
extern unsigned long getMillis();
extern void setLed(int led, bool state,
                   bool allocationFailure);

extern unsigned long previousMillisYellow;

// PUT /config/mode active of passive in msg body
// PUT /config/cbuffsize int waarde in msg body
// DELETE /sensors/1 verwijder alle waarden voor 1|2
// POST /sensors/1 ontvang int meetwaarde voor 1|2
// GET /sensors/1/avg geef μ voor 1|2 als in 38.2
// GET /sensors/2/stdev geef σ voor 1|2 als in 5.3
// GET /sensors/1/actual geef m uit buffer als in 37.3
const char* HTTP_OK = "HTTP/1.1 200 OK\r\n";
const char* HTTP_OK_BODY =
    "HTTP/1.1 200 OK\r\nContent-Length: ";
const char* HTTP_OK_CHANGE = "HTTP/1.1 201 Created\r\n";
const char* HTTP_BAD_REQUEST =
    "HTTP/1.1 400 Bad Request\r\n";

const char* ALLOWED_ENDPOINTS[] = {"/config/mode",
                                   "/config/cbuffsize",
                                   "/sensors/1",
                                   "/sensors/2",
                                   "/sensors/1/avg",
                                   "/sensors/2/avg",
                                   "/sensors/1/stdev",
                                   "/sensors/2/stdev",
                                   "/sensors/1/actual",
                                   "/sensors/2/actual",
                                   "/"};

struct AllowedCall {
  const char* call;
  void (*handler)(const char* body);
};

void init_cserver(CircularBuffer* buffer1,
                  CircularBuffer* buffer2) {
  init_buffer(buffer1, 12);
  init_buffer(buffer2, 12);
  buffer_1 = buffer1;
  buffer_2 = buffer2;

  activeMode = false;
  allocationFailure = false;

  aggregate1.count = 0;
  aggregate1.mean = 0;
  aggregate1.M2 = 0;

  aggregate2.count = 0;
  aggregate2.mean = 0;
  aggregate2.M2 = 0;
}

void handlePutConfigMode(const char* body) {
  // printToSerial("Handling PUT /config/mode");
  //  Check if body is "active" or "passive"
  if (strcmp(body, "active") == 0) {
    activeMode = true;
    // printToSerial("Setting mode to active");
  } else if (strcmp(body, "passive") == 0) {
    // printToSerial("Setting mode to passive");
    activeMode = false;
  } else {
    printToClient(HTTP_BAD_REQUEST);
    return;
  }
  printToClient(HTTP_OK_CHANGE);
}

void handlePutConfigCbuffsize(const char* body) {
  // printToSerial("Handling PUT /config/cbuffsize");
  //  Check if body is a valid integer
  int cbuffsize = atoi(body);
  if (cbuffsize > 0) {
    // Set the buffer size
    if (resize_buffer(buffer_1, cbuffsize) &&
        resize_buffer(buffer_2, cbuffsize)) {
      printToClient(HTTP_OK_CHANGE);
      return;
    } else {
      allocationFailure = true;
      printToClient(
          "HTTP/1.1 500 Internal Server Error\r\n");
      return;
    }
  } else {
    printToClient(HTTP_BAD_REQUEST);
    return;
  }
  // Add logic to handle this call
}

void handleDeleteSensors1(const char* body) {
  // printToSerial("Handling DELETE /sensors/1");
  empty_buffer(buffer_1);
  reset_aggregate(&aggregate1);

  printToClient(HTTP_OK_CHANGE);
}

void handleDeleteSensors2(const char* body) {
  // printToSerial("Handling DELETE /sensors/2");
  empty_buffer(buffer_2);
  reset_aggregate(&aggregate2);

  printToClient(HTTP_OK_CHANGE);
}

void handlePostSensors1(const char* body) {
  // printToSerial("Handling POST /sensors/1");
  int num;
  char extra;

  // Check if body is a valid integer
  if (sscanf(body, "%d%c", &num, &extra) != 1) {
    printToSerial("Invalid input\n");
    printToClient(HTTP_BAD_REQUEST);
  } else {
    printToSerial("Valid input");
    printToSerialInt(num);
    printToSerial("");

    update_aggregate(&aggregate1, num);
    insert_buffer(buffer_1, num);

    printToClient(HTTP_OK_CHANGE);
  }
}

void handlePostSensors2(const char* body) {
  // printToSerial("Handling POST /sensors/2");
  int num;
  char extra;

  // Check if body is a valid integer
  if (sscanf(body, "%d%c", &num, &extra) != 1) {
    printToSerial("Invalid input\n");
    printToClient(HTTP_BAD_REQUEST);
  } else {
    printToSerial("Valid input");
    printToSerialInt(num);
    printToSerial("");

    update_aggregate(&aggregate2, num);
    insert_buffer(buffer_2, num);

    printToClient(HTTP_OK_CHANGE);
  }
}

void handleGetSensors1Avg(const char* body) {
  // printToSerial("Handling GET /sensors/1/avg");
  char averageBuffer1Str[16] = {
      0}; // Allocate a buffer for the
  // float-to-string conversion

  double mean = 0;
  double variance = 0;
  double sample_variance = 0;
  double stddev = 0;
  finalize_aggregate(&aggregate1, &mean, &variance,
                     &sample_variance, &stddev);

  dtostrf(mean, 1, 1, averageBuffer1Str);
  int contentLength = strlen(averageBuffer1Str);

  printToClient(HTTP_OK_BODY);
  printToClientInt(contentLength);
  printToClient("\r\n\r\n");
  printToClient(averageBuffer1Str);
  printToSerial(averageBuffer1Str);
}

void handleGetSensors2Avg(const char* body) {
  // printToSerial("Handling GET /sensors/2/avg");
  char averageBuffer2Str[16] = {
      0}; // Allocate a buffer for the
  // float-to-string conversion
  double mean = 0;
  double variance = 0;
  double sample_variance = 0;
  double stddev = 0;
  finalize_aggregate(&aggregate2, &mean, &variance,
                     &sample_variance, &stddev);
  dtostrf(mean, 1, 1, averageBuffer2Str);
  int contentLength = strlen(averageBuffer2Str);

  printToClient(HTTP_OK_BODY);
  printToClientInt(contentLength);
  printToClient("\r\n\r\n");
  printToClient(averageBuffer2Str);
  printToSerial(averageBuffer2Str);
}

void handleGetSensors1Stdev(const char* body) {
  // printToSerial("Handling GET /sensors/1/stdev");
  char stdDevSensor1Str[16] = {
      0}; // Allocate a buffer for the
          // float-to-string conversion
  double mean = 0;
  double variance = 0;
  double sample_variance = 0;
  double stddev = 0;
  finalize_aggregate(&aggregate1, &mean, &variance,
                     &sample_variance, &stddev);
  printToSerial("stddev: ");
  printToSerialFloat(stddev);
  printToSerial("variance: ");
  printToSerialFloat(variance);
  dtostrf(stddev, 1, 1, stdDevSensor1Str);
  int contentLength = strlen(stdDevSensor1Str);

  printToClient(HTTP_OK_BODY);
  printToClientInt(contentLength);
  printToClient("\r\n\r\n");
  printToClient(stdDevSensor1Str);
}

void handleGetSensors2Stdev(const char* body) {
  // printToSerial("Handling GET /sensors/2/stdev");
  char stdDevSensor2Str[16] = {
      0}; // Allocate a buffer for the

  double mean = 0;
  double variance = 0;
  double sample_variance = 0;
  double stddev = 0;
  finalize_aggregate(&aggregate2, &mean, &variance,
                     &sample_variance, &stddev);
  printToSerial("stddev: ");
  printToSerialFloat(stddev);
  printToSerial("variance: ");
  printToSerialFloat(variance);
  dtostrf(stddev, 1, 1, stdDevSensor2Str);
  int contentLength = strlen(stdDevSensor2Str);

  printToClient(HTTP_OK_BODY);
  printToClientInt(contentLength);
  printToClient("\r\n\r\n");
  printToClient(stdDevSensor2Str);
}

void handleGetSensors1Actual(const char* body) {
  // printToSerial("Handling GET /sensors/2/actual");
  //  Add logic to handle this call
  float averageBuffer1 = get_mean(buffer_1);
  printToSerialFloat(averageBuffer1);
  printToSerial("");

  // Prepare the response body
  char averageBuffer1Str[16] = {0};
  // float-to-string conversion
  dtostrf(averageBuffer1, 1, 1, averageBuffer1Str);
  int contentLength = strlen(averageBuffer1Str);

  // empty the buffer
  empty_buffer(buffer_1);

  printToClient(HTTP_OK_BODY);
  printToClientInt(contentLength);
  printToClient("\r\n\r\n");
  printToClient(averageBuffer1Str);
}

void handleGetSensors2Actual(const char* body) {
  // printToSerial("Handling GET /sensors/2/actual");
  //  Add logic to handle this call
  float averageBuffer2 = get_mean(buffer_2);

  // Prepare the response body
  char averageBuffer2Str[16] = {
      0}; // Allocate a buffer for the
          // float-to-string conversion
  dtostrf(averageBuffer2, 1, 1, averageBuffer2Str);
  int contentLength = strlen(averageBuffer2Str);

  // empty the buffer and aggregate
  empty_buffer(buffer_1);

  printToClient(HTTP_OK_BODY);
  printToClientInt(contentLength);
  printToClient("\r\n\r\n");
  printToClient(averageBuffer2Str);
}

void handleOnlineCheck(const char* body) {
  printToClient(HTTP_OK);
}

struct AllowedCall ALLOWED_CALLS[] = {
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
    {"GET /sensors/2/actual", handleGetSensors2Actual},
    {"GET /", handleOnlineCheck}};

void handleRequest(struct stream stream) {
  // Response received from the client. Yellow LED on for
  // 0.5 seconds
  setLed(YELLOW, true, allocationFailure);
  previousMillisYellow = getMillis();

  char buffer[256] = {0};
  int bufferIndex = 0;

  while (stream.available() > 0 &&
         bufferIndex < sizeof(buffer) - 1) {
    buffer[bufferIndex++] = stream.read();
  }
  buffer[bufferIndex] = '\0';

  // Check for the line that contains Content-Length:
  char* contentLengthLine =
      strstr(buffer, "Content-Length:");
  int contentLength = 0;
  char* body = NULL;
  if (contentLengthLine != NULL) {
    // Get the content length value
    char* contentLengthValue =
        contentLengthLine + strlen("Content-Length: ");
    contentLength = atoi(contentLengthValue);

    // Get the last line (body)
    char* lastLine = strrchr(buffer, '\n');
    body = lastLine + 1;
    int bodyLength = strlen(body);
  }

  // Get the first line (e.g., "PUT /config/mode HTTP/1.1")
  // and remove the HTTP version
  char* firstLine = strtok(buffer, "\n");
  char* method = strtok(firstLine, " ");
  char* target = strtok(NULL, " ");

  // Check if target is in ALLOWED_ENDPOINTS
  bool targetAllowed = false;
  for (int i = 0; i < sizeof(ALLOWED_ENDPOINTS) /
                          sizeof(ALLOWED_ENDPOINTS[0]);
       i++) {
    if (strcmp(target, ALLOWED_ENDPOINTS[i]) == 0) {
      targetAllowed = true;
      break;
    }
  }
  if (!targetAllowed) {
    printToClient("HTTP/1.1 404 Not Found\r\n");
    return;
  }

  // Check if method is in ALLOWED_CALLS
  char fullCall[64] = {0}; // Statically allocate a buffer
                           // for the full call
  snprintf(fullCall, sizeof(fullCall), "%s %s", method,
           target);

  bool methodAllowed = false;
  for (int iterator = 0;
       iterator <
       sizeof(ALLOWED_CALLS) / sizeof(ALLOWED_CALLS[0]);
       iterator++) {
    if (strcmp(fullCall, ALLOWED_CALLS[iterator].call) ==
        0) {
      methodAllowed = true;
      // Call the corresponding handler function
      ALLOWED_CALLS[iterator].handler(body);
      return;
    }
  }

  if (!methodAllowed) {
    printToClient(HTTP_BAD_REQUEST);
  }
}
void setBuffer1(CircularBuffer* buffer) {
  buffer_1 = buffer;
}
void setBuffer2(CircularBuffer* buffer) {
  buffer_2 = buffer;
}
