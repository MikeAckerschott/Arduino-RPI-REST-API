#ifndef cserver_h
#define cserver_h

#include "stream.h"
#include <stdbool.h> // ik denk dat deze hier overbodig is
#include "buffermock.h"

CircularBuffer *buffer_1;
CircularBuffer *buffer_2;

void handleRequest(struct stream);

void setBuffer1 (CircularBuffer * buffer1);
void setBuffer2 (CircularBuffer * buffer2);


#endif
