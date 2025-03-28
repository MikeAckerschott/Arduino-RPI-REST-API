#include "buffermock.h"

bool init_buffer(CircularBuffer *buffer, int size) {
    buffer->data = (int *)malloc(size * sizeof(int));
    if (buffer->data == NULL) {
        return false;
    }
    buffer->oldest = 0;
    buffer->newest = -1;
    buffer->size = size;
    buffer->count = 0;
    return true;
}

void insert_buffer(CircularBuffer *buffer, int value) {
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

bool resize_buffer(CircularBuffer *buffer, int new_size) {
    int *new_array = (int *)malloc(new_size * sizeof(int));
    if (new_array == NULL || new_size <= 0) {
        return false;
    }

    int copy_count = (buffer->count < new_size) ? buffer->count : new_size;
    for (int i = 0; i < copy_count; i++) {
        new_array[i] = buffer->data[(buffer->oldest + i) % buffer->size];
    }

    free(buffer->data);

    buffer->data = new_array;
    buffer->size = new_size;
    buffer->oldest = 0;
    buffer->newest = copy_count - 1;
    buffer->count = copy_count;

    return true;
}

void empty_buffer(CircularBuffer *buffer) {
    for(int i = 0; i < buffer->count; i++) {
        buffer->data[(buffer->oldest + i) % buffer->size] = 0;
    }
    buffer->oldest = 0;
    buffer->newest = -1;
    buffer->count = 0;
}

double get_mean(CircularBuffer *buffer) {
    if (buffer->count == 0) {
        return 0.0; // Avoid division by zero
    }
    double sum = 0.0;
    for (int i = 0; i < buffer->count; i++) {
        sum += buffer->data[(buffer->oldest + i) % buffer->size];
    }
    return ((int)(sum / buffer->count * 10)) / 10.0; // Round to one decimal
}