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
  req->success = false;
  req->method = METHOD_UNRECOGNIZED;
  req->target[0] = TARGET_UNRECOGNIZED;
  req->target[1] = TARGET_UNRECOGNIZED;
  req->target[2] = TARGET_UNRECOGNIZED;
  req->field = FIELD_UNRECOGNIZED;
  req->body = BODY_EMPTY;
  req->content_length = 0;
  req->body_int = 0;
}

void parseRequest(struct request *req, const char *buffer)
{
  // Use a pointer to traverse the buffer directly
  const char *line = buffer;

  // Find the first newline character to isolate the first line
  const char *newline = strchr(line, '\n');
  if (newline != NULL)
  {
    // Temporarily null-terminate the first line
    size_t lineLength = newline - line;
    char firstLine[lineLength + 1];
    strncpy(firstLine, line, lineLength);
    firstLine[lineLength] = '\0';

    // Parse the method and target from the first line
    char *method = strtok(firstLine, " ");
    parse_method(req, method);
    char *target = strtok(NULL, " ");
    parse_target(req, target);
  }

  // Find the Content-Length field
  const char *contentLength = strstr(buffer, "Content-Length: ");
  if (contentLength != NULL)
  {
    // Parse the content length
    contentLength += strlen("Content-Length: ");
    req->content_length = atoi(contentLength);
    req->field = FIELD_CONTENT_LENGTH;
  }
  else
  {
    req->content_length = 0;
    req->field = FIELD_UNRECOGNIZED;
  }

  // Find the body
  char *body = strstr(buffer, "\r\n\r\n");
  if (body != NULL)
  {
    // Parse the body
    body += strlen("\r\n\r\n");
    if (req->content_length == 0)
    {
      req->body = BODY_EMPTY;
    }
    else if (strncmp(body, "passive", 7) == 0)
    {
      req->body = BODY_PASSIVE;
    }
    else if (strncmp(body, "active", 6) == 0)
    {
      req->body = BODY_ACTIVE;
    }
    else
    {
      printToSerial(body);
      printToSerialInt(strlen(body));

      // Body can be an int presented as "1", "2", "3"... "100" etc.
      // Check if the body is a valid integer
      char *endptr;
      strtol(body, &endptr, 10);
      if (endptr != body && *endptr == '\0')
      {
        req->body = BODY_INT;
        req->body_int = atoi(body);
      }
      else
      {
        req->body = BODY_UNRECOGNIZED;
        printToSerial("UNRECOGNIZED body - body is not an int");
      }
    }
  }
  else
  {
    req->body = BODY_UNRECOGNIZED;
    printToSerial("UNRECOGNIZED body - body is NULL");
  }

  printToSerial(buffer);
}

void parse_method(struct request *req, const char *method)
{
  if (strcmp(method, "GET") == 0)
  {
    printToSerial("GET method");
    req->method = METHOD_GET;
  }
  else if (strcmp(method, "POST") == 0)
  {
    printToSerial("POST method");
    req->method = METHOD_POST;
  }
  else if (strcmp(method, "PUT") == 0)
  {
    printToSerial("PUT method");
    req->method = METHOD_PUT;
  }
  else if (strcmp(method, "DELETE") == 0)
  {
    printToSerial("DELETE method");
    req->method = METHOD_DELETE;
  }
  else
  {
    printToSerial("UNRECOGNIZED method");
    req->method = METHOD_UNRECOGNIZED;
  }
}

// splitting on the '/' would have been better, but for this amount of endpoints doing that much string manipulation is not worth it
void parse_target(struct request *req, const char *target)
{
  if (strcmp(target, "/config/mode") == 0)
  {
    req->target[0] = TARGET_CONFIG;
    req->target[1] = TARGET_MODE;
  }
  else if (strcmp(target, "/config/cbuffsize") == 0)
  {
    req->target[0] = TARGET_CONFIG;
    req->target[1] = TARGET_CBUFFSIZE;
  }
  else if (strcmp(target, "/sensors/1/avg") == 0)
  {
    req->target[0] = TARGET_SENSORS;
    req->target[1] = TARGET_1;
    req->target[2] = TARGET_AVG;
  }
  else if (strcmp(target, "/sensors/1/stdev") == 0)
  {
    req->target[0] = TARGET_SENSORS;
    req->target[1] = TARGET_1;
    req->target[2] = TARGET_STDEV;
  }
  else if (strcmp(target, "/sensors/2/avg") == 0)
  {
    req->target[0] = TARGET_SENSORS;
    req->target[1] = TARGET_2;
    req->target[2] = TARGET_AVG;
  }
  else if (strcmp(target, "/sensors/2/stdev") == 0)
  {
    req->target[0] = TARGET_SENSORS;
    req->target[1] = TARGET_2;
    req->target[2] = TARGET_STDEV;
  }
  else if (strcmp(target, "/sensors/1/actual") == 0)
  {
    req->target[0] = TARGET_SENSORS;
    req->target[1] = TARGET_1;
    req->target[2] = TARGET_ACTUAL;
  }
  else if (strcmp(target, "/sensors/2/actual") == 0)
  {
    req->target[0] = TARGET_SENSORS;
    req->target[1] = TARGET_2;
    req->target[2] = TARGET_ACTUAL;
  }
  else
  {
    req->target[0] = TARGET_UNRECOGNIZED;
  }
}

#endif
