#ifndef abnf_h
#define abnf_h

#include "cserver.h"
#include "stream.h"
#include <stdbool.h>

bool http_message(struct stream stream);

#endif