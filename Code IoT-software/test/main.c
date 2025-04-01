#include <stdio.h>
#include <math.h>
#include "welford_online.h"

int main() {
    // Test data
    int values[] = {457, 285, 209, 178, 864, 65, 61, 191, 447, 476, 54, 407, 859, 501};
    int num_values = sizeof(values) / sizeof(values[0]);
    double expected_mean = 361.0;
    double expected_stdev = 255.74820206043063;

    // Initialize WelfordAggregate
    WelfordAggregate aggregate = {0, 0.0, 0.0};

    // Update the aggregate with each value
    for (int i = 0; i < num_values; i++) {
        update(&aggregate, values[i]);
    }

    // Finalize and retrieve results
    double mean, variance, sample_variance;
    if (finalize(&aggregate, &mean, &variance, &sample_variance)) {
        double stdev = sqrt(variance);

        // Print results
        printf("Calculated Mean: %.2f\n", mean);
        printf("Calculated Standard Deviation: %.15f\n", stdev);

        // Validate results
        if (fabs(mean - expected_mean) <= 0.1 && fabs(stdev - expected_stdev) <= 0.1) {
            printf("Test Passed!\n");
        } else {
            printf("Test Failed!\n");
            printf("Expected Mean: %.2f, Expected Standard Deviation: %.15f\n", expected_mean, expected_stdev);
            printf("Calculated Mean: %.2f, Calculated Standard Deviation: %.15f\n", mean, stdev);
        }
    } else {
        printf("Not enough data points to calculate statistics.\n");
    }

    return 0;
}