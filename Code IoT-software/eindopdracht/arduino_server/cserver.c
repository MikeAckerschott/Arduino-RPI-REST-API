#include "cserver.h"
#include "abnf.h"
#include "arduino_wrapper.h"
#include "request.h"

extern void printToSerial(const char *message);
extern unsigned long getMillis();
extern void setLed(int led, bool state);

extern unsigned long previousMillisYellow;

struct response handleRequest(struct stream stream)
{
  // response received from the client. yellow led on for 0.5 seconds
  setLed(YELLOW, true);
  previousMillisYellow = getMillis();

  struct response response;
  char buffer[256] = {0};
  int bufferIndex = 0;

  while (stream.available() > 0 && bufferIndex < sizeof(buffer) - 1)
  {
    buffer[bufferIndex++] = stream.read();
  }
  buffer[bufferIndex] = '\0';

  struct request request;
  init_request(&request);
  parseRequest(&request, buffer);

  // route toelichting
  // PUT /config/mode active of passive in msg body
  // PUT /config/cbuffsize int waarde in msg body
  // DELETE /sensors/1 verwijder alle waarden voor 1|2
  // POST /sensors/1 ontvang int meetwaarde voor 1|2
  // GET /sensors/1/avg geef μ voor 1|2 als in 38.2
  // GET /sensors/2/stdev geef σ voor 1|2 als in 5.3
  // GET /sensors/1/actual geef m uit buffer als in 37.3

  bool configModePath = request.target[0] == TARGET_CONFIG && request.target[1] == TARGET_MODE;
  bool configCbuffsizePath = request.target[0] == TARGET_CONFIG && request.target[1] == TARGET_CBUFFSIZE;
  bool sensorPath = request.target[0] == TARGET_SENSORS && (request.target[1] == TARGET_1 || request.target[1] == TARGET_2);
  bool sensorAveragePath = sensorPath && request.target[2] == TARGET_AVG;
  bool sensorStdevPath = sensorPath && request.target[2] == TARGET_STDEV;
  bool sensorActualPath = sensorPath && request.target[2] == TARGET_ACTUAL;

  // check if the request has a valid endpoint
  if (!configModePath && !configCbuffsizePath && !sensorPath && !sensorAveragePath && !sensorStdevPath && !sensorActualPath)
  {
    //no valid endpoint reached
    response.code = NOT_FOUND_404;
    printToSerial("no valid endpoint reached");
    return response;
  }

  bool correctConfigMode = configModePath && request.method == METHOD_PUT && request.body == BODY_PASSIVE || request.body == BODY_ACTIVE;
  bool correctConfigCbuffsize = configCbuffsizePath && request.method == METHOD_PUT && request.body == BODY_INT && request.body_int > 0;
  bool correctBufferDeletion = sensorPath && request.method == METHOD_DELETE;
  bool correctBufferInsertion = sensorPath && request.method == METHOD_POST && request.body == BODY_INT;
  bool correctBufferAvg = sensorAveragePath && request.method == METHOD_GET;
  bool correctBufferStdev = sensorStdevPath && request.method == METHOD_GET;
  bool correctBufferActual = sensorActualPath && request.method == METHOD_GET;

  printToSerialInt(request.method);
  printToSerialInt(request.target[0]);
  printToSerialInt(request.target[1]);
  printToSerialInt(request.target[2]);
  printToSerialInt(request.body);
  printToSerialInt(request.body_int);
  printToSerialInt(request.content_length);
  printToSerialInt(request.field);

  printToSerial("");
  printToSerialInt(correctConfigMode);
  printToSerialInt(correctConfigCbuffsize);
  printToSerialInt(correctBufferDeletion);
  printToSerialInt(correctBufferInsertion);
  printToSerialInt(correctBufferAvg);
  printToSerialInt(correctBufferStdev);
  printToSerialInt(correctBufferActual);


  // check if the request has valid syntax
  if (!correctConfigMode && !correctConfigCbuffsize && !correctBufferDeletion && !correctBufferInsertion && !correctBufferAvg && !correctBufferStdev && !correctBufferActual)
  {
    //valid endpoint reached but incorrect method or body
    response.code = BAD_REQUEST_400;
    printToSerial("valid endpoint reached but incorrect method or body");
    return response;
  }

  if (correctConfigMode && request.body == BODY_PASSIVE)
  {
    response.code = CREATED_201_PUT_MODE_PASSIVE;
  }
  else if (correctConfigMode && request.body == BODY_ACTIVE)
  {
    response.code = CREATED_201_PUT_MODE_ACTIVE;
  }
  else if (correctConfigCbuffsize)
  {
    // resize the buffer
    if (resize_buffer(buffer_1, request.body_int) && resize_buffer(buffer_2, request.body_int))
    {
      response.code = CREATED_201_PUT_CBUFFSIZE;
    }
    else
    {
      response.code = INTERNAL_SERVER_ERROR_500;
    }
  }
  else if (correctBufferDeletion)
  {
    // delete the buffer
    empty_buffer(buffer_1);
    empty_buffer(buffer_2);
    response.code = CREATED_201_DELETE_MEASUREMENTS;
  }
  else if (correctBufferInsertion)
  {
    // insert into the buffer
    if (request.target[1] == TARGET_1)
    {
      insert_buffer(buffer_1, request.body_int);
    }
    else
    {
      insert_buffer(buffer_2, request.body_int);
    }

    response.code = CREATED_201_POST_MEASUREMENT;
  }
  else if (correctBufferAvg)
  {
    // get the average from the buffer
    if (request.target[1] == TARGET_1){
      // response.get_avg = get_mean(buffer_1);
    }
    else{
      // response.get_avg = get_mean(buffer_2);
    }

    response.code = OK_200_GET_AVG;
  }
  else if (correctBufferStdev)
  {
    // get the standard deviation from the buffer
    if (request.target[1] == TARGET_1)
    {
      // response.get_stdev = get_stdev(buffer_1);
    }
    else
    {
      // response.get_stdev = get_stdev(buffer_2);
    }

    response.code = OK_200_GET_STDEV;
  }
  else if (correctBufferActual)
  {
    // get the actual value from the buffer
    if (request.target[1] == TARGET_1)
    {
      // response.get_actual = get_actual_value(buffer_1);
    }
    else
    {
      // response.get_actual = get_actual_value(buffer_2);
    }

    response.code = OK_200_GET_ACTUAL;
  } else {
    // no valid endpoint reached
    printToSerial("no valid endpoint reached. THIS SHOULD NOT BE REACHED!");
    response.code = BAD_REQUEST_400;
  }
  return response;
}

void setBuffer1(CircularBuffer *buffer)
{
  buffer_1 = buffer;
}
void setBuffer2(CircularBuffer *buffer)
{
  buffer_2 = buffer;
}
