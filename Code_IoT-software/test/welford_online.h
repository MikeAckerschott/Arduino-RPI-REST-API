#ifndef WELFORD_ONLINE_H
#define WELFORD_ONLINE_H

// Define the WelfordAggregate structure
typedef struct {
    int count;       // Number of samples
    double mean;     // Running mean
    double M2;       // Sum of squared differences from the mean
} WelfordAggregate;

// Function to update the aggregate with a new value
void update(WelfordAggregate *aggregate, double new_value);

// Function to finalize and retrieve the mean, variance, and sample variance
int finalize(WelfordAggregate *aggregate, double *mean, double *variance, double *sample_variance);

#endif