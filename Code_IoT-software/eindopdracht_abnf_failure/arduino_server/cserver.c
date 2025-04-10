#include "cserver.h"
#include "abnf.h"
#include "arduino_wrapper.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void printToClient(const char* message);
extern void printToClientInt(const int message);
extern void printToSerial(const char* message);
extern unsigned long getMillis();
extern void setLed(int led, bool state,
                   bool allocationFailure);

extern unsigned long previousMillisYellow;

const char* HTTP_OK = "HTTP/1.1 200 OK\r\n";
const char* HTTP_OK_BODY =
    "HTTP/1.1 200 OK\r\nContent-Length: ";
const char* HTTP_OK_CHANGE = "HTTP/1.1 201 Created\r\n";
const char* HTTP_BAD_REQUEST =
    "HTTP/1.1 400 Bad Request\r\n";

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
  //  Check if body is "active" or "passive"
  if (strcmp(body, "active") == 0) {
    activeMode = true;
  } else if (strcmp(body, "passive") == 0) {
    activeMode = false;
  } else {
    printToClient(HTTP_BAD_REQUEST);
    return;
  }
  printToClient(HTTP_OK_CHANGE);
}

void handlePutConfigCbuffsize(const char* body) {
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
      // printToSerial("Allocation failure\n");
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
  empty_buffer(buffer_1);
  reset_aggregate(&aggregate1);

  printToClient(HTTP_OK_CHANGE);
}

void handleDeleteSensors2(const char* body) {
  empty_buffer(buffer_2);
  reset_aggregate(&aggregate2);

  printToClient(HTTP_OK_CHANGE);
}

void handlePostSensors1(const char* body) {
  int num;
  char extra;

  printToSerial(body);

  // Check if body is a valid integer
  if (sscanf(body, "%d%c", &num, &extra) != 1) {
    printToClient(HTTP_BAD_REQUEST);
  } else {

    update_aggregate(&aggregate1, num);
    insert_buffer(buffer_1, num);

    printToClient(HTTP_OK_CHANGE);
  }
}

void handlePostSensors2(const char* body) {
  int num;
  char extra;

  // Check if body is a valid integer
  if (sscanf(body, "%d%c", &num, &extra) != 1) {
    printToClient(HTTP_BAD_REQUEST);
  } else {

    update_aggregate(&aggregate2, num);
    insert_buffer(buffer_2, num);

    printToClient(HTTP_OK_CHANGE);
  }
}

void handleSensors1(const char* body) {
  if (incoming_request.method == METHOD_POST) {
    handlePostSensors1(body);
  } else if (incoming_request.method == METHOD_DELETE) {
    handleDeleteSensors1(body);
  } else {
    printToSerial("BAD METHOD");
    printToSerialInt(incoming_request.method);
    printToClient(HTTP_BAD_REQUEST);
  }
}
void handleSensors2(const char* body) {
  if (incoming_request.method == METHOD_POST) {
    handlePostSensors2(body);
  } else if (incoming_request.method == METHOD_DELETE) {
    handleDeleteSensors2(body);
  } else {
    printToClient(HTTP_BAD_REQUEST);
  }
}

void handleGetSensors1Avg(const char* body) {
  char averageBuffer1Str[8] = {
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
}

void handleGetSensors2Avg(const char* body) {
  char averageBuffer2Str[8] = {
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
}

void handleGetSensors1Stdev(const char* body) {
  char stdDevSensor1Str[16] = {
      0}; // Allocate a buffer for the
          // float-to-string conversion
  double mean = 0;
  double variance = 0;
  double sample_variance = 0;
  double stddev = 0;
  finalize_aggregate(&aggregate1, &mean, &variance,
                     &sample_variance, &stddev);
  dtostrf(stddev, 1, 1, stdDevSensor1Str);
  int contentLength = strlen(stdDevSensor1Str);

  printToClient(HTTP_OK_BODY);
  printToClientInt(contentLength);
  printToClient("\r\n\r\n");
  printToClient(stdDevSensor1Str);
}

void handleGetSensors2Stdev(const char* body) {
  char stdDevSensor2Str[16] = {
      0}; // Allocate a buffer for the

  double mean = 0;
  double variance = 0;
  double sample_variance = 0;
  double stddev = 0;
  finalize_aggregate(&aggregate2, &mean, &variance,
                     &sample_variance, &stddev);
  dtostrf(stddev, 1, 1, stdDevSensor2Str);
  int contentLength = strlen(stdDevSensor2Str);

  printToClient(HTTP_OK_BODY);
  printToClientInt(contentLength);
  printToClient("\r\n\r\n");
  printToClient(stdDevSensor2Str);
}

void handleGetSensors1Actual(const char* body) {
  //  Add logic to handle this call
  float averageBuffer1 = get_mean(buffer_1);

  // Prepare the response body
  char averageBuffer1Str[8] = {0};
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
  //  Add logic to handle this call
  float averageBuffer2 = get_mean(buffer_2);

  // Prepare the response body
  char averageBuffer2Str[8] = {
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

void handleRequest(struct stream stream) {
  // Response received from the client. Yellow LED on for
  // 0.5 seconds
  // printToSerial("handleRequest\n");

  if (http_message(stream)) {
    // printToSerial("HTTP message received\n");

    setLed(YELLOW, true, allocationFailure);
    previousMillisYellow = getMillis();

    // printToSerialInt(incoming_request.method);
    // printToSerialInt(incoming_request.target);
    // printToSerialInt(incoming_request.field);
    // printToSerial(incoming_request.body);

    if (incoming_request.target == TARGET_UNRECOGNIZED) {
      printToClient(HTTP_BAD_REQUEST);
      printToSerial("TARGET_UNRECOGNIZED\n");
      return;
    } else {
      ENDPOINTS_CONFIG[incoming_request.target].handler(
          incoming_request.body);
      printToSerial("Handler called\n");
      printToSerial(
          ENDPOINTS_CONFIG[incoming_request.target].value);
      return;
    }
  } else {
    printToSerial("HTTP message failed\n");
  }

  printToClient(HTTP_BAD_REQUEST);
}

void clearRequest() {
  incoming_request.method = METHOD_UNRECOGNIZED;
  incoming_request.target = TARGET_UNRECOGNIZED;
  incoming_request.field = FIELD_UNRECOGNIZED;
  for (int i = 0; i < sizeof(incoming_request.body); ++i) {
    incoming_request.body[i] = '\0';
  }
}