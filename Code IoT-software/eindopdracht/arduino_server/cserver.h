#ifndef cserver_h
#define cserver_h

#include "buffermock.h"
#include "stream.h"
#include "welford_online.h"
#include <stdbool.h> // ik denk dat deze hier overbodig is

CircularBuffer* buffer_1;
CircularBuffer* buffer_2;

WelfordAggregate aggregate1;
WelfordAggregate aggregate2;

// BOOLEAN FOR ALLOCATION FAILURE
bool allocationFailure;

// BOOLEAN FOR ACTIVE AND PASSIVE MODE
bool activeMode;

void init_cserver(CircularBuffer* buffer1,
                  CircularBuffer* buffer2);

void handleRequest(struct stream);

void setBuffer1(CircularBuffer* buffer1);
void setBuffer2(CircularBuffer* buffer2);

#endif
