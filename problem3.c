#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* (i) Early-exit bubble sort: stops early if the array is sorted */
long bubbleSortEarlyExit(int arr[], int n) {
    long comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                int t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
                swapped = 1;
            }
        }
        if (!swapped) break;   /* already sorted -> terminate early */
    }
    return comparisons;
}

/* (ii) Full bubble sort: always completes all (n-1) passes */
long bubbleSortFull(int arr[], int n) {
    long comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                int t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
        }
        /* no early-exit check -- always does the full (n-1) passes */
    }
    return comparisons;
}

/* Fill arr[0..n-1] with a random permutation-ish sequence of values */
void generateRandomArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % (n * 10 + 1);
    }
}

int main(void) {
    srand((unsigned int) time(NULL));

    int sizes[] = {100, 200, 400, 800, 1000, 2000, 4000, 6000, 8000, 10000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int trialsPerSize = 5;   /* average over several random arrays */

    printf("n,EarlyExit_Comparisons,FullPass_Comparisons\n");

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int *original = malloc(n * sizeof(int));
        int *work     = malloc(n * sizeof(int));

        long totalEarly = 0, totalFull = 0;

        for (int t = 0; t < trialsPerSize; t++) {
            generateRandomArray(original, n);

            for (int i = 0; i < n; i++) work[i] = original[i];
            totalEarly += bubbleSortEarlyExit(work, n);

            for (int i = 0; i < n; i++) work[i] = original[i];
            totalFull  += bubbleSortFull(work, n);
        }

        long avgEarly = totalEarly / trialsPerSize;
        long avgFull  = totalFull  / trialsPerSize;

        printf("%d,%ld,%ld\n", n, avgEarly, avgFull);

        free(original);
        free(work);
    }

    return 0;
}