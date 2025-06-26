#include "buffermock.h"

extern void setLed(int led, bool state,
                   bool allocationFailure);

bool init_buffer(CircularBuffer* buffer, int size) {
  buffer->data = (int*)malloc(size * sizeof(int));
  if (buffer->data == NULL) {
    return false;
  }
  buffer->oldest = 0;
  buffer->newest = -1;
  buffer->size = size;
  buffer->count = 0;
  return true;
}

void insert_buffer(CircularBuffer* buffer, int value) {
  if (buffer->count < buffer->size) {
    buffer->newest = (buffer->newest + 1) % buffer->size;
    buffer->data[buffer->newest] = value;
    buffer->count++;
  } else {
    // Overwrite the oldest value
    buffer->newest = (buffer->newest + 1) % buffer->size;
    buffer->data[buffer->newest] = value;
    buffer->oldest = (buffer->oldest + 1) % buffer->size;
  }
}

bool resize_buffer(CircularBuffer* buffer, int new_size) {
  if (new_size <= 0) {
    return false; // Invalid size
  }
  int* new_data = (int*)malloc(new_size * sizeof(int));
  if (new_data == NULL) {
    setLed(RED1, true, false);
    setLed(RED2, true, false);
    setLed(YELLOW, true, false);
    setLed(GREEN, true, false);
    return false; // Allocation failed
  }
  int count =
      buffer->count < new_size ? buffer->count : new_size;
  for (int i = 0; i < count; i++) {
    new_data[i] =
        buffer->data[(buffer->oldest + i) % buffer->size];
  }

  // Free old data and assign new data
  if (buffer->data != NULL) {
    free(buffer->data);
  }
  buffer->data = new_data;
  buffer->oldest = 0;
  buffer->newest = count - 1;
  buffer->size = new_size;
  buffer->count = count;

  return true;
}

void empty_buffer(CircularBuffer* buffer) {
  for (int i = 0; i < buffer->count; i++) {
    buffer->data[(buffer->oldest + i) % buffer->size] = 0;
  }
  buffer->oldest = 0;
  buffer->newest = -1;
  buffer->count = 0;
}

float get_mean(CircularBuffer* buffer) {
  if (buffer->count == 0) {
    return -1; // Avoid division by zero
  }
  float sum = 0.0;
  for (int i = 0; i < buffer->count; i++) {
    sum +=
        buffer->data[(buffer->oldest + i) % buffer->size];
  }
  return ((int)(sum / buffer->count * 10)) /
         10.0; // Round to one decimal
}