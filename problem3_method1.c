#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Standard 2-way merge: merges A[0..sizeA-1] and B[0..sizeB-1]
   (both sorted ascending) into a newly-malloc'd sorted array. */
int *merge2(int A[], int sizeA, int B[], int sizeB, long *comparisons) {
    int *out = malloc((sizeA + sizeB) * sizeof(int));
    int i = 0, j = 0, k = 0;
    while (i < sizeA && j < sizeB) {
        (*comparisons)++;
        if (A[i] <= B[j]) out[k++] = A[i++];
        else               out[k++] = B[j++];
    }
    while (i < sizeA) out[k++] = A[i++];
    while (j < sizeB) out[k++] = B[j++];
    return out;
}

/* METHOD 1: merge arrays[0] with arrays[1], then merge that result
   with arrays[2], then with arrays[3], ..., until arrays[k-1]. */
int *mergeKArraysMethod1(int *arrays[], int n, int k, long *comparisons) {
    int *result = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) result[i] = arrays[0][i];
    int resultSize = n;

    for (int a = 1; a < k; a++) {
        int *merged = merge2(result, resultSize, arrays[a], n, comparisons);
        free(result);
        result = merged;
        resultSize += n;
    }
    return result;
}

/* helpers */
int *makeSortedRandomArray(int n, int rangeMultiplier) {
    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = rand() % (n * rangeMultiplier + 1);
    /* simple insertion sort (n is small per-array; fine for building test data) */
    for (int i = 1; i < n; i++) {
        int key = arr[i], j = i - 1;
        while (j >= 0 && arr[j] > key) { arr[j + 1] = arr[j]; j--; }
        arr[j + 1] = key;
    }
    return arr;
}

int isSorted(int arr[], int n) {
    for (int i = 1; i < n; i++) if (arr[i - 1] > arr[i]) return 0;
    return 1;
}

int main(void) {
    srand((unsigned int) time(NULL));

    /* ---- Quick correctness demo ---- */
    int a1[] = {1, 4, 7};
    int a2[] = {2, 3, 9};
    int a3[] = {0, 5, 6};
    int *demoArrays[] = {a1, a2, a3};
    long demoCmp = 0;
    int *demoResult = mergeKArraysMethod1(demoArrays, 3, 3, &demoCmp);
    printf("Demo: merging [1,4,7], [2,3,9], [0,5,6] -> ");
    for (int i = 0; i < 9; i++) printf("%d ", demoResult[i]);
    printf("\n\n");
    free(demoResult);

    /* ---- Experiment A: fix n, vary k -- should show ~quadratic growth in k ---- */
    printf("Experiment A: n fixed = 200, k varies\n");
    printf("k,n,Comparisons\n");
    int nFixed = 200;
    int kValues[] = {2, 4, 8, 16, 32, 64, 128};
    int numK = sizeof(kValues) / sizeof(kValues[0]);
    for (int t = 0; t < numK; t++) {
        int k = kValues[t];
        int **arrays = malloc(k * sizeof(int *));
        for (int i = 0; i < k; i++) arrays[i] = makeSortedRandomArray(nFixed, 100);

        long cmp = 0;
        int *result = mergeKArraysMethod1(arrays, nFixed, k, &cmp);
        if (!isSorted(result, nFixed * k)) fprintf(stderr, "ERROR: not sorted for k=%d\n", k);

        printf("%d,%d,%ld\n", k, nFixed, cmp);

        free(result);
        for (int i = 0; i < k; i++) free(arrays[i]);
        free(arrays);
    }

    /* ---- Experiment B: fix k, vary n -- should show linear growth in n ---- */
    printf("\nExperiment B: k fixed = 16, n varies\n");
    printf("k,n,Comparisons\n");
    int kFixed = 16;
    int nValues[] = {100, 500, 1000, 5000, 10000, 20000};
    int numN = sizeof(nValues) / sizeof(nValues[0]);
    for (int t = 0; t < numN; t++) {
        int n = nValues[t];
        int **arrays = malloc(kFixed * sizeof(int *));
        for (int i = 0; i < kFixed; i++) arrays[i] = makeSortedRandomArray(n, 100);

        long cmp = 0;
        int *result = mergeKArraysMethod1(arrays, n, kFixed, &cmp);
        if (!isSorted(result, n * kFixed)) fprintf(stderr, "ERROR: not sorted for n=%d\n", n);

        printf("%d,%d,%ld\n", kFixed, n, cmp);

        free(result);
        for (int i = 0; i < kFixed; i++) free(arrays[i]);
        free(arrays);
    }

    printf("\nConclusion: Method 1 (repeatedly merging in one array at a\n");
    printf("time) costs O(n*k^2) in the worst case: quadratic in the\n");
    printf("number of arrays k, but only linear in the per-array size n.\n");
    printf("Doubling k roughly QUADRUPLES the comparisons (Experiment A),\n");
    printf("while doubling n roughly DOUBLES the comparisons (Experiment B).\n");

    return 0;
}