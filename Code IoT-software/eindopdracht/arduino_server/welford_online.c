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
                       double* sample_variance,
                       double* stddev) {

  if (aggregate->count == 0) {
    *mean = -1;
    *variance = -1;
    *sample_variance = -1;
    *stddev = -1;
    return 0;
  }

  *mean = aggregate->mean;
  *variance = aggregate->M2 / aggregate->count;

  if (aggregate->count == 1) {
    *sample_variance = 0;
    *stddev = 0;
    return 0;
  }

  *sample_variance = aggregate->M2 / (aggregate->count - 1);
  *stddev = sqrt(*variance);
  return 1; // Success
}

void reset_aggregate(WelfordAggregate* aggregate) {
  aggregate->count = 0;
  aggregate->mean = 0.0;
  aggregate->M2 = 0.0;
}