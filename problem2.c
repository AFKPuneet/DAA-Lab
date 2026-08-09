#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Simulate `tosses` tosses of a coin with P(HEAD) = p.
   Returns the empirical probability of HEAD (heads / tosses). */
double simulateCoin(long tosses, double p) {
    long heads = 0;
    for (long i = 0; i < tosses; i++) {
        double r = (double) rand() / ((double) RAND_MAX + 1.0); /* uniform [0,1) */
        if (r < p) {
            heads++;
        }
    }
    return (double) heads / (double) tosses;
}

int main(void) {
    srand((unsigned int) time(NULL));

    double fairP   = 0.5;   /* fair coin   */
    double biasedP = 0.75;  /* biased coin: P(HEAD) = 0.75 (change as needed) */

    long trialSizes[] = {10, 100, 1000, 10000, 100000, 1000000, 10000000};
    int numTrials = sizeof(trialSizes) / sizeof(trialSizes[0]);

    printf("---------------------------------------------------------\n");
    printf("Part A: Fair coin (true P(HEAD) = %.2f)\n", fairP);
    printf("---------------------------------------------------------\n");
    printf("%-12s %-20s\n", "Tosses", "Empirical P(HEAD)");
    for (int i = 0; i < numTrials; i++) {
        double empP = simulateCoin(trialSizes[i], fairP);
        printf("%-12ld %-20.5f\n", trialSizes[i], empP);
    }

    printf("\n---------------------------------------------------------\n");
    printf("Part B: Fair vs Biased coin (true P(HEAD) = %.2f)\n", biasedP);
    printf("---------------------------------------------------------\n");
    printf("%-12s %-20s %-20s\n", "Tosses", "Fair P(HEAD)", "Biased P(HEAD)");
    for (int i = 0; i < numTrials; i++) {
        double empFair   = simulateCoin(trialSizes[i], fairP);
        double empBiased = simulateCoin(trialSizes[i], biasedP);
        printf("%-12ld %-20.5f %-20.5f\n", trialSizes[i], empFair, empBiased);
    }

    printf("\nObservation: as the number of tosses grows, the empirical\n");
    printf("probability for each coin converges to its TRUE underlying\n");
    printf("probability (0.5 for the fair coin, %.2f for the biased coin),\n", biasedP);
    printf("in line with the Law of Large Numbers.\n");

    return 0;
}