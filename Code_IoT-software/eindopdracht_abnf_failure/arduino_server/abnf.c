#include "abnf.h"
#include "request.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void printToClient(const char* message);
extern void printToClientInt(const int message);
extern void printToSerial(const char* message);

static bool field_line(struct stream);
static bool http_token(struct stream);
static bool http_version(struct stream);
static bool message_body(struct stream);
static bool method(struct stream);
static bool origin_form(struct stream);
static bool request_line(struct stream);
static bool request_target(struct stream);
static bool start_line(struct stream);
static bool tchar(struct stream, char[], size_t);

static bool readTokenType(struct stream, enum tokentype);
static bool readTokenValue(struct stream, enum tokentype,
                           const char*);
// catValue is not static to allow for an informative test
void catValue(const char*, char[], size_t);

bool http_message(struct stream stream) {
  initTokenizer();

  // ABNF for http_message is an AND construction, so early
  // exit on error

  if (!start_line(stream)) {
    // printToSerial("start_line failed\n");
    return false;
  }

  if (!readTokenType(stream, CRLF)) {

    // printToSerial("\n");
    //  printToSerial("CRLF failed\n");
    return false;
  }

  // this is optional. it contains the content length or
  // other fields?
  if (peekToken(stream).type != CRLF) {

    while (field_line(stream) &&
           readTokenType(stream, CRLF)) {
      // back to back crlf means body follows
      if (peekToken(stream).type == CRLF) {
        break;
      }

      // no body found.
      if (stream.available() == 0) {
        printToSerial("empty");
        break;
      }
      printToSerialInline(peekToken(stream).value);
    }
  }

  if (!readTokenType(stream, CRLF)) {
    printToSerial("CRLF 2 failed\n");
    return false;
  }

  if (!message_body(stream)) {
    printToSerial("message_body failed\n");
    return false;
  }

  printToSerial("message_body succeeded\n");

  return true;
}

// remaining ABFN functions are stubs

static bool field_line(struct stream stream) {

  while ((peekToken(stream).type == VCHAR ||
          peekToken(stream).type == DIGIT ||
          peekToken(stream).type == ALPHA ||
          peekToken(stream).type == SP) &&
         stream.available()) {
    readToken(
        stream); // Consume the token to avoid infinite loop
  }

  if (peekToken(stream).type == CRLF) {
    return true;
  }

  return false;
}

static bool http_token(struct stream stream) {
  char result[20] = {0};
  for (int i = 0; i < 7; ++i) {
    if (!tchar(stream, result, sizeof(result))) {
      // printf("tchar failed at iteration %d\n", i);
      // printf("tchar: token type: '%d'\n",
      // peekToken(stream).type); printf("tchar: token
      // value: '%s'\n", peekToken(stream).value);
      break;
    }
  }
  if (strcmp(result, "GET") == 0) {
    incoming_request.method = METHOD_GET;
  } else if (strcmp(result, "POST") == 0) {
    incoming_request.method = METHOD_POST;
  } else if (strcmp(result, "PUT") == 0) {
    incoming_request.method = METHOD_PUT;
  } else if (strcmp(result, "DELETE") == 0) {
    incoming_request.method = METHOD_DELETE;
  } else {
    // printf("HTTP_TOKEN: '%s' not recognized\n", result);
    incoming_request.method = METHOD_UNRECOGNIZED;
    return false;
  }
  // printToSerial("HTTP_TOKEN: ");
  // printToSerial(result);
  // printToSerialInt(incoming_request.method);
  return true;
}

static bool http_version(struct stream stream) {
  if (!readTokenValue(stream, ALPHA, "HTTP")) {
    // printf("HTTP failed\n");
    return false;
  }

  if (!readTokenValue(stream, VCHAR, "/")) {
    // printf("/ failed\n");
    return false;
  }

  if (!readTokenType(stream, DIGIT)) {
    // printf("DIGIT failed\n");
    return false;
  }

  if (!readTokenValue(stream, VCHAR, ".")) {
    // printf(". failed\n");
    return false;
  }

  if (!readTokenType(stream, DIGIT)) {
    // printf("DIGIT failed\n");
    return false;
  }

  return true;
}

static bool message_body(struct stream stream) {
  // no message body
  if (stream.available() == 0) {
    return true;
  }

  struct token next = peekToken(stream);
  char result[20] = {0};

  // body allows alot?
  while ((next.type == VCHAR || next.type == DIGIT ||
          next.type == ALPHA || next.type == SP) &&
         stream.available()) {
    next = readToken(stream);
    catValue(next.value, result, sizeof(result));
  }

  // full body checked on character validity. return true
  if (stream.available() == 0) {

    for (int i = 0; i < strlen(result); ++i) {
      incoming_request.body[i] = result[i];
    }

    return true;
  }

  // unallowed character in body
  return false;
}

static bool method(struct stream stream) {
  return http_token(stream);
}

static bool request_line(struct stream stream) {
  if (!method(stream)) {
    // printf("method failed");
    return false;
  }

  // printf("method succeeded.. CHECK SP\n");
  if (!readTokenValue(stream, SP, " ")) {
    // printf("SP failed");
    return false;
  }

  if (!request_target(stream)) {
    // printf("request_target failed");
    return false;
  }

  if (!readTokenValue(stream, SP, " ")) {
    // printf("SP2 failed\n");
    return false;
  }

  if (!http_version(stream)) {
    // printf("http_version failed");
    return false;
  }

  return true;
}

static bool request_target(struct stream stream) {
  return origin_form(stream);
}

static bool origin_form(struct stream stream) {
  char result[20] = {0};

  while (peekToken(stream).type == VCHAR ||
         peekToken(stream).type == DIGIT ||
         peekToken(stream).type == ALPHA ||
         peekToken(stream).type == SP) {
    if (peekToken(stream).type == SP) {
      for (int i = 0; i < (sizeof(ENDPOINTS_CONFIG) /
                           sizeof(ENDPOINTS_CONFIG[0]));
           i++) {
        if (strcmp(result, ENDPOINTS_CONFIG[i].value) ==
            0) {
          // printToSerial("ALLOWED");
          incoming_request.target =
              ENDPOINTS_CONFIG[i].target;
          return true;
        }
      }
      incoming_request.target = TARGET_UNRECOGNIZED;
      return false;
    }

    catValue(peekToken(stream).value, result,
             sizeof(result));

    readToken(stream);
  }
  incoming_request.target = TARGET_UNRECOGNIZED;
  return false;
}

static bool start_line(struct stream stream) {
  return request_line(stream);
}

static bool tchar(struct stream stream, char result[],
                  size_t len) {
  struct token next = peekToken(stream);

  if (next.type == DIGIT) {
    next = readToken(stream);
    catValue(next.value, result, len);
    return true;
  }

  if (next.type == ALPHA) {
    next = readToken(stream);
    catValue(next.value, result, len);
    return true;
  }

  if (next.type == VCHAR) {
    switch (next.value[0]) {
    case '!': // falls through on purpose
    case '#':
    case '$':
    case '%':
    case '&':
    case '\'':
    case '*':
    case '+':
    case '-':
    case '.':
    case '^':
    case '`':
    case '|':
    case '~':
      next = readToken(stream);
      catValue(next.value, result, len);
      return true;
    }
  }

  return false;
}

static bool readTokenType(struct stream stream,
                          enum tokentype type) {
  struct token next = peekToken(stream);
  // printf("peeked: token type: '%d'\n", next.type);
  // printf("peeked: token value: '%s'\n", next.value);
  if (!hasTokenType(next, type)) {
    printToSerialInline("'");
    printToSerialInline(next.value);
    printToSerialInline("'");
    printToSerialInt(next.type);
    printToSerialInt(stream.available());
    return false;
  }

  readToken(stream);
  return true;
}

static bool readTokenValue(struct stream stream,
                           enum tokentype type,
                           const char* value) {
  struct token complete = peekToken(stream);

  for (int i = 0; i < strlen(value); ++i) {
    struct token next = readToken(stream);
    complete.value[i] = next.value[0];
  }

  if (complete.type != type) {
    return false;
  }

  if (strncmp(complete.value, value, strlen(value)) != 0) {
    return false;
  }

  return true;
}

void catValue(const char* value, char result[],
              size_t len) {
  if (len == 0) {
    return;
  }
  strncat(result, value, len - strlen(result) - 1);
}