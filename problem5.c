#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Linear scan: O(n) worst case */
int findPartitionLinear(int A[], int n, long *comparisons) {
    *comparisons = 0;
    for (int i = 0; i < n; i++) {
        (*comparisons)++;
        if (A[i] == 1) return i;
    }
    return n;   /* no 1 found -> all zeros */
}

/* Binary search: O(log n) worst case
   Invariant: A[lo..] could contain the first 1; everything before lo is 0. */
int findPartitionBinarySearch(int A[], int n, long *comparisons) {
    *comparisons = 0;
    int lo = 0, hi = n;   /* hi = n means "no 1 found yet" */

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        (*comparisons)++;
        if (A[mid] == 1) {
            hi = mid;        /* first 1 is at mid or earlier */
        } else {
            lo = mid + 1;    /* first 1 is strictly after mid */
        }
    }
    return lo;   /* lo == hi == index of first 1 (or n if all zeros) */
}

/* Build a test array of n elements: a run of `zeros` 0's followed by
   (n - zeros) 1's. */
void buildArray(int A[], int n, int zeros) {
    for (int i = 0; i < n; i++) {
        A[i] = (i < zeros) ? 0 : 1;
    }
}

int main(void) {
    srand((unsigned int) time(NULL));

    /* --- Quick correctness demo --- */
    int demo[] = {0, 0, 0, 0, 0, 1, 1, 1};
    int demoN = sizeof(demo) / sizeof(demo[0]);
    long cmp;
    int idx = findPartitionBinarySearch(demo, demoN, &cmp);
    printf("Demo array: 0 0 0 0 0 1 1 1\n");
    printf("Partition point (first index of 1) = %d  (binary search used %ld comparisons)\n\n",
           idx, cmp);

    /* --- Compare linear vs binary search comparison counts --- */
    int sizes[] = {100, 1000, 10000, 100000, 1000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("n,LinearComparisons,BinarySearchComparisons\n");
    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int *A = malloc(n * sizeof(int));

        /* Worst case for linear scan: transition happens near the end
           (put the 1's starting roughly 3/4 of the way through). */
        int zeros = (3 * n) / 4;
        buildArray(A, n, zeros);

        long cmpLinear, cmpBinary;
        int r1 = findPartitionLinear(A, n, &cmpLinear);
        int r2 = findPartitionBinarySearch(A, n, &cmpBinary);

        printf("%d,%ld,%ld\n", n, cmpLinear, cmpBinary);

        if (r1 != r2) {
            printf("  ERROR: mismatch! linear=%d binary=%d\n", r1, r2);
        }

        free(A);
    }

    printf("\nConclusion: linear scan takes O(n) comparisons in the worst\n");
    printf("case, while binary search (exploiting the fact that the array\n");
    printf("is already partitioned into 0's then 1's) takes only O(log n)\n");
    printf("comparisons -- a huge saving for large n.\n");

    return 0;
}