#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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

/* METHOD 2: pair up arrays and merge each pair; repeat on the
   resulting (roughly halved) set of arrays until one remains.
   `arrays` and `sizes` are consumed/replaced round by round. */
int *mergeKArraysMethod2(int *arrays[], int sizes[], int k, long *comparisons, int *outSize) {
    /* work on local copies of the pointer/size arrays so we can
       freely reassign them each round without disturbing the caller */
    int **curArrays = malloc(k * sizeof(int *));
    int *curSizes = malloc(k * sizeof(int));
    for (int i = 0; i < k; i++) { curArrays[i] = arrays[i]; curSizes[i] = sizes[i]; }
    int curK = k;

    while (curK > 1) {
        int nextK = (curK + 1) / 2;         /* ceil(curK/2): handles odd counts */
        int **nextArrays = malloc(nextK * sizeof(int *));
        int *nextSizes = malloc(nextK * sizeof(int));

        int idx = 0;
        for (int i = 0; i + 1 < curK; i += 2) {
            nextArrays[idx] = merge2(curArrays[i], curSizes[i], curArrays[i + 1], curSizes[i + 1], comparisons);
            nextSizes[idx] = curSizes[i] + curSizes[i + 1];
            idx++;
        }
        if (curK % 2 == 1) {                /* odd one out just carries forward untouched */
            nextArrays[idx] = curArrays[curK - 1];
            nextSizes[idx] = curSizes[curK - 1];
            idx++;
        }

        /* free only the arrays that were actually merged (i.e. all except a
           possible carried-forward last one, which we must NOT free here) */
        int mergedCount = (curK / 2) * 2;
        for (int i = 0; i < mergedCount; i++) free(curArrays[i]);

        free(curArrays);
        free(curSizes);
        curArrays = nextArrays;
        curSizes = nextSizes;
        curK = nextK;
    }

    int *result = curArrays[0];
    *outSize = curSizes[0];
    free(curArrays);
    free(curSizes);
    return result;
}

/* helpers */
int *makeSortedRandomArray(int n, int rangeMultiplier) {
    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = rand() % (n * rangeMultiplier + 1);
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
    /* (arrays passed to mergeKArraysMethod2 get freed as they're merged in,
       so we must pass malloc'd copies, not raw stack arrays) */
    int a1src[] = {1, 4, 7};
    int a2src[] = {2, 3, 9};
    int a3src[] = {0, 5, 6};
    int *a1 = malloc(3 * sizeof(int)); for (int i = 0; i < 3; i++) a1[i] = a1src[i];
    int *a2 = malloc(3 * sizeof(int)); for (int i = 0; i < 3; i++) a2[i] = a2src[i];
    int *a3 = malloc(3 * sizeof(int)); for (int i = 0; i < 3; i++) a3[i] = a3src[i];
    int *demoArrays[] = {a1, a2, a3};
    int demoSizes[] = {3, 3, 3};
    long demoCmp = 0;
    int demoOutSize;
    int *demoResult = mergeKArraysMethod2(demoArrays, demoSizes, 3, &demoCmp, &demoOutSize);
    printf("Demo: merging [1,4,7], [2,3,9], [0,5,6] -> ");
    for (int i = 0; i < demoOutSize; i++) printf("%d ", demoResult[i]);
    printf("\n\n");
    free(demoResult);

    /* ---- Experiment A: fix n, vary k -- should show ~n*k*log(k) growth ---- */
    printf("Experiment A: n fixed = 200, k varies\n");
    printf("k,n,Comparisons,nk_log2k_prediction\n");
    int nFixed = 200;
    int kValues[] = {2, 4, 8, 16, 32, 64, 128};
    int numK = sizeof(kValues) / sizeof(kValues[0]);
    for (int t = 0; t < numK; t++) {
        int k = kValues[t];
        int **arrays = malloc(k * sizeof(int *));
        int *sizes = malloc(k * sizeof(int));
        for (int i = 0; i < k; i++) { arrays[i] = makeSortedRandomArray(nFixed, 100); sizes[i] = nFixed; }

        long cmp = 0;
        int outSize;
        int *result = mergeKArraysMethod2(arrays, sizes, k, &cmp, &outSize);
        if (!isSorted(result, outSize)) fprintf(stderr, "ERROR: not sorted for k=%d\n", k);

        double prediction = (double) nFixed * k * (log((double) k) / log(2.0));
        printf("%d,%d,%ld,%.1f\n", k, nFixed, cmp, prediction);

        free(result);
        free(arrays);
        free(sizes);
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
        int *sizes = malloc(kFixed * sizeof(int));
        for (int i = 0; i < kFixed; i++) { arrays[i] = makeSortedRandomArray(n, 100); sizes[i] = n; }

        long cmp = 0;
        int outSize;
        int *result = mergeKArraysMethod2(arrays, sizes, kFixed, &cmp, &outSize);
        if (!isSorted(result, outSize)) fprintf(stderr, "ERROR: not sorted for n=%d\n", n);

        printf("%d,%d,%ld\n", kFixed, n, cmp);

        free(result);
        free(arrays);
        free(sizes);
    }

    printf("\nConclusion: Method 2 (pairwise/tournament merging) costs\n");
    printf("O(n*k*log k) in the worst case -- there are log2(k) rounds,\n");
    printf("and each round touches all nk elements exactly once. This is\n");
    printf("asymptotically much better than Method 1's O(n*k^2): the k^2\n");
    printf("factor is replaced by k*log(k), a dramatic improvement for\n");
    printf("large k (e.g. k=1024: k^2 ~ 10^6 vs k*log2(k) ~ 10^4).\n");

    return 0;
}