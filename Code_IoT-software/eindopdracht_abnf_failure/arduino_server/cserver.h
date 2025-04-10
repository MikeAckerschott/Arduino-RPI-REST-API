#ifndef cserver_h
#define cserver_h

#include "buffermock.h"
#include "request.h"
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
void clearRequest();
void handlePutConfigMode(const char* body);
void handlePutConfigCbuffsize(const char* body);
void handleDeleteSensors1(const char* body);
void handleDeleteSensors2(const char* body);
void handlePostSensors1(const char* body);
void handlePostSensors2(const char* body);
void handleSensors1(const char* body);
void handleSensors2(const char* body);
void handleGetSensors1Avg(const char* body);
void handleGetSensors2Avg(const char* body);
void handleGetSensors1Stdev(const char* body);
void handleGetSensors2Stdev(const char* body);
void handleGetSensors1Actual(const char* body);
void handleGetSensors2Actual(const char* body);
void handleOnlineCheck(const char* body);

#endif
