#ifndef abnf_h
#define abnf_h

#include "stream.h"
#include <stdbool.h>
#include "request.h"

struct request incoming_request;

bool http_message(struct stream stream);

#endif