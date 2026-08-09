#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ---------------------------- Standard merge sort ---------------------------- */

void merge2(int arr[], int l, int m, int r, long *comparisons) {
    int n1 = m - l + 1, n2 = r - m;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        (*comparisons)++;
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else               arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r, long *comparisons) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m, comparisons);
    mergeSort(arr, m + 1, r, comparisons);
    merge2(arr, l, m, r, comparisons);
}

/* ---------------------------- Modified (3-way) merge sort ---------------------------- */

/* 3-way merge of arr[l..m1], arr[m1+1..m2], arr[m2+1..r] */
void merge3(int arr[], int l, int m1, int m2, int r, long *comparisons) {
    int n1 = m1 - l + 1, n2 = m2 - m1, n3 = r - m2;
    int *A = malloc(n1 * sizeof(int));
    int *B = malloc(n2 * sizeof(int));
    int *C = malloc(n3 * sizeof(int));
    for (int i = 0; i < n1; i++) A[i] = arr[l + i];
    for (int i = 0; i < n2; i++) B[i] = arr[m1 + 1 + i];
    for (int i = 0; i < n3; i++) C[i] = arr[m2 + 1 + i];

    int i = 0, j = 0, k = 0, idx = l;
    while (i < n1 && j < n2 && k < n3) {
        (*comparisons) += 2;   /* two comparisons to find the min of three */
        if (A[i] <= B[j] && A[i] <= C[k])      arr[idx++] = A[i++];
        else if (B[j] <= A[i] && B[j] <= C[k]) arr[idx++] = B[j++];
        else                                    arr[idx++] = C[k++];
    }
    /* at most one of the three runs remains -> finish with a 2-way merge-style drain */
    while (i < n1 && j < n2) { (*comparisons)++; arr[idx++] = (A[i] <= B[j]) ? A[i++] : B[j++]; }
    while (i < n1 && k < n3) { (*comparisons)++; arr[idx++] = (A[i] <= C[k]) ? A[i++] : C[k++]; }
    while (j < n2 && k < n3) { (*comparisons)++; arr[idx++] = (B[j] <= C[k]) ? B[j++] : C[k++]; }
    while (i < n1) arr[idx++] = A[i++];
    while (j < n2) arr[idx++] = B[j++];
    while (k < n3) arr[idx++] = C[k++];

    free(A); free(B); free(C);
}

void mergeSort3Way(int arr[], int l, int r, long *comparisons) {
    if (r - l < 1) return;                 /* 0 or 1 element: already sorted */
    int len = r - l + 1;
    int m1 = l + len / 3 - 1;
    int m2 = l + 2 * (len / 3) - 1;
    if (m1 < l) m1 = l;                    /* guard for very small subarrays */
    if (m2 <= m1) m2 = m1 + 1;
    if (m2 > r) m2 = r;

    mergeSort3Way(arr, l, m1, comparisons);
    mergeSort3Way(arr, m1 + 1, m2, comparisons);
    mergeSort3Way(arr, m2 + 1, r, comparisons);
    merge3(arr, l, m1, m2, r, comparisons);
}

/* ---------------------------- helpers ---------------------------- */

void fillRandom(int arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = rand() % (n * 10 + 1);
}

int isSorted(int arr[], int n) {
    for (int i = 1; i < n; i++) if (arr[i - 1] > arr[i]) return 0;
    return 1;
}

int main(void) {
    srand((unsigned int) time(NULL));

    int sizes[] = {100, 500, 1000, 5000, 10000, 50000, 100000, 500000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("n,MergeSort2Way_Comparisons,MergeSort3Way_Comparisons\n");

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int *original = malloc(n * sizeof(int));
        int *work = malloc(n * sizeof(int));
        fillRandom(original, n);

        for (int i = 0; i < n; i++) work[i] = original[i];
        long cmp2 = 0;
        mergeSort(work, 0, n - 1, &cmp2);
        if (!isSorted(work, n)) fprintf(stderr, "ERROR: 2-way merge sort failed for n=%d\n", n);

        for (int i = 0; i < n; i++) work[i] = original[i];
        long cmp3 = 0;
        mergeSort3Way(work, 0, n - 1, &cmp3);
        if (!isSorted(work, n)) fprintf(stderr, "ERROR: 3-way merge sort failed for n=%d\n", n);

        printf("%d,%ld,%ld\n", n, cmp2, cmp3);

        free(original);
        free(work);
    }

    printf("\nConclusion: both curves grow as Theta(n log n) -- the modified\n");
    printf("(3-way) merge sort does NOT change the asymptotic order of\n");
    printf("growth. It only changes the constant factor: fewer recursion\n");
    printf("levels (log base 3 instead of log base 2) but a more expensive\n");
    printf("per-level merge (up to 2 comparisons per output element instead\n");
    printf("of 1). These effects roughly cancel, so both remain O(n log n).\n");

    return 0;
}