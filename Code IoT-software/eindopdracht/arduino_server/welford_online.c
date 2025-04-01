#include "welford_online.h"

// For a new value new_value, compute the new count, new
// mean, and the new M2. mean accumulates the mean of the
// entire dataset M2 aggregates the squared distance from
// the mean count aggregates the number of samples seen so
// far

extern void printToSerial(const char* message);
extern void printToSerialInt(const int message);
extern void printToSerialFloat(const float message);

void update_aggregate(WelfordAggregate* aggregate,
                      double new_value) {
  aggregate->count += 1;
  double delta = new_value - aggregate->mean;
  aggregate->mean += delta / aggregate->count;
  double delta2 = new_value - aggregate->mean;
  aggregate->M2 += delta * delta2;

  printToSerial("MEASUREMENT:");
  printToSerialInt(aggregate->count);
  printToSerialFloat(aggregate->mean);
  printToSerialFloat(aggregate->M2);
  printToSerial("");
}

// Retrieve the mean, variance, and sample variance from an
// aggregate
int finalize_aggregate(WelfordAggregate* aggregate,
                       double* mean, double* variance,
                       double* sample_variance) {
  if (aggregate->count < 2) {
    return 0; // Not enough data points
    printToSerial("NOT ENOUGH DATAPOINTS!");
  } else {
    printToSerial("FINALIZE: ");
    printToSerialInt(aggregate->count);
    printToSerialFloat(aggregate->mean);
    printToSerialFloat(aggregate->M2);

    *mean = aggregate->mean;
    *variance = aggregate->M2 / aggregate->count;
    *sample_variance =
        aggregate->M2 / (aggregate->count - 1);
    printToSerialFloat(*variance);
    printToSerial("");
    return 1; // Success
  }
}

void reset_aggregate(WelfordAggregate* aggregate) {
  aggregate->count = 0;
  aggregate->mean = 0.0;
  aggregate->M2 = 0.0;
}