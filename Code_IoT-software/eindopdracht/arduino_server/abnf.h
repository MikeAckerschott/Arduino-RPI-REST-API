#ifndef abnf_h
#define abnf_h

#include "cserver.h"
#include "request.h"
#include "stream.h"
#include <stdbool.h>

struct request incoming_request;

bool http_message(struct stream stream);

#endif