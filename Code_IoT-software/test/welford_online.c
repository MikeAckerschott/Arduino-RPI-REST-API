#include "welford_online.h"

// For a new value new_value, compute the new count, new mean, and the new M2.
// mean accumulates the mean of the entire dataset
// M2 aggregates the squared distance from the mean
// count aggregates the number of samples seen so far

void update(WelfordAggregate *aggregate, double new_value) {
    aggregate->count += 1;
    double delta = new_value - aggregate->mean;
    aggregate->mean += delta / aggregate->count;
    double delta2 = new_value - aggregate->mean;
    aggregate->M2 += delta * delta2;
}

// Retrieve the mean, variance, and sample variance from an aggregate
int finalize(WelfordAggregate *aggregate, double *mean, double *variance, double *sample_variance) {
    if (aggregate->count < 2) {
        return 0; // Not enough data points
    } else {
        *mean = aggregate->mean;
        *variance = aggregate->M2 / aggregate->count;
        *sample_variance = aggregate->M2 / (aggregate->count - 1);
        return 1; // Success
    }
}