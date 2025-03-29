#ifndef cserver_h
#define cserver_h

#include "stream.h"
#include <stdbool.h> // ik denk dat deze hier overbodig is
#include "buffermock.h"

CircularBuffer *buffer_1;
CircularBuffer *buffer_2;

// AVERAGES AND STANDARD DEVIATIONS
unsigned long totalReadings;
unsigned long totalSensor1;
unsigned long totalSensor2;
unsigned long totalSquaredSensor1;
unsigned long totalSquaredSensor2;
float averageSensor1;
float averageSensor2;
float stdDevSensor1;
float stdDevSensor2;

// BOOLEAN FOR ACTIVE AND PASSIVE MODE
bool activeMode;

void init_cserver(CircularBuffer *buffer1, CircularBuffer *buffer2);

void handleRequest(struct stream);

void setBuffer1(CircularBuffer *buffer1);
void setBuffer2(CircularBuffer *buffer2);

#endif
