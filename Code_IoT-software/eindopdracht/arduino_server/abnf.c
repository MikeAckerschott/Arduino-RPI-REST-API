#include "abnf.h"
#include "cserver.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool field_line(struct stream);
static bool field_name(struct stream);
static bool field_value(struct stream);
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
    // printf("start_line failed\n");
    return false;
  }

  // printf("START_LINE succeeded. CHECK CRLF\n");
  if (!readTokenType(stream, CRLF)) {

    // printf("\n");
    // printf("CRLF failed\n");
    return false;
  }

  // this is optional. it contains the content length or
  // other fields?
  if (!field_line(stream)) {
    return false;
  }

  if (!readTokenType(stream, CRLF)) {
    // printf("CRLF 2 failed\n");
    return false;
  }

  if (!message_body(stream)) {
    return false;
  }

  return true;
}

// remaining ABFN functions are stubs

static bool field_line(struct stream stream) {
  struct token next = peekToken(stream);

  while ((next.type == VCHAR || next.type == DIGIT ||
          next.type == ALPHA || next.type == SP) &&
         stream.available()) {
    next = readToken(stream);
  }

  if (peekToken(stream).type == CRLF) {
    return true;
  }

  return false;
}

static bool field_name(struct stream stream) {
  char result[256] = {0};
  return tchar(stream, result, sizeof(result));
}

static bool field_value(struct stream stream) {
  struct token next = peekToken(stream);

  if (next.type == VCHAR || next.type == SP ||
      next.type == HTAB) {
    readToken(stream);
    return true;
  }

  return false;
}

static bool http_token(struct stream stream) {
  // request_method[6] = {0};
  memset(request_method, 0, sizeof(request_method));
  for (int i = 0; i < 7; ++i) {
    if (!tchar(stream, request_method,
               sizeof(request_method))) {
      // printf("tchar failed at iteration %d\n", i);
      // printf("tchar: token type: '%d'\n",
      // peekToken(stream).type); printf("tchar: token
      // value: '%s'\n", peekToken(stream).value);
      break;
    }
  }
  // printToSerial("HTTP_TOKEN: ");
  // printToSerial(request_method);
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
  // request_body[16] = {0};
  memset(request_body, 0, sizeof(request_body));

  // body allows alot?
  while ((next.type == VCHAR || next.type == DIGIT ||
          next.type == ALPHA || next.type == SP)) {
    struct token current = readToken(stream);
    catValue(current.value, request_body,
             sizeof(request_body));
    next = peekToken(stream);
  }

  // full body checked on character validity. return true
  if (stream.available() == 0) {
    // printToSerial("BODY      :");
    // printToSerial(request_body);
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
  struct token next = peekToken(stream);
  // request_endpoint[20] = {0};
  memset(request_endpoint, 0, sizeof(request_endpoint));

  while (next.type == VCHAR || next.type == DIGIT ||
         next.type == ALPHA || next.type == SP) {
    if (peekToken(stream).type == SP) {
      // printToSerial("ENDPOINT  :");
      // printToSerial(request_endpoint);
      return true;
    }
    struct token current = readToken(stream);
    catValue(current.value, request_endpoint,
             sizeof(request_endpoint));

    next = peekToken(stream);
  }

  // printf("origin_form failed\n");
  // printf("origin_form: token type: '%d'\n", next.type);
  // printf("origin_form: token value: '%s'\n", next.value);
  // printf("origin_form: expected type: '%d', '%d',
  // '%d'\n", VCHAR, DIGIT, ALPHA);

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