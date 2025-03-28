#ifndef request_h
#define request_h

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

enum method
{
  METHOD_UNRECOGNIZED = 0,
  METHOD_GET = 1,
  METHOD_POST = 2,
  METHOD_PUT = 3,
  METHOD_DELETE = 4
};

enum target
{
  TARGET_UNRECOGNIZED = 0,
  TARGET_CONFIG = 1,
  TARGET_MODE = 2,
  TARGET_CBUFFSIZE = 3,
  TARGET_SENSORS = 4,
  TARGET_1 = 5,
  TARGET_2 = 6,
  TARGET_AVG = 7,
  TARGET_STDEV = 8,
  TARGET_ACTUAL = 9
};

enum field
{
  FIELD_UNRECOGNIZED = 0,
  FIELD_CONTENT_LENGTH = 1
};

enum body
{
  BODY_EMPTY = 0,
  BODY_UNRECOGNIZED = 1,
  BODY_PASSIVE = 2,
  BODY_ACTIVE = 3,
  BODY_INT = 4
};

struct request
{
  bool success;

  enum method method;
  enum target target[3];
  enum field field;
  enum body body;

  int16_t content_length;

  int16_t body_int; // in case body == BODY_INT
};

// Function to initialize the struct with default values
void init_request(struct request *req)
{

}

void parseRequest(struct request *req, const char *buffer)
{
  //printToSerial(buffer); 
}

#endif
