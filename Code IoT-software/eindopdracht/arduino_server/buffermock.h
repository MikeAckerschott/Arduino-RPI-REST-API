#ifndef BUFFERMOCK_H
#define BUFFERMOCK_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    int *data;
    int newest;
    int oldest;
    int size;
    int count;
} CircularBuffer;

// Function prototypes
bool init_buffer(CircularBuffer *buffer, int size);
void insert_buffer(CircularBuffer *buffer, int value);
bool resize_buffer(CircularBuffer *buffer, int new_size);
void empty_buffer(CircularBuffer *buffer);

#endif