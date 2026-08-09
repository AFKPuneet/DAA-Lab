#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* (i) Naive O(n^2) uniqueness check */
int hasDuplicateNaive(int A[], int n, long *comparisons) {
    *comparisons = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            (*comparisons)++;
            if (A[i] == A[j]) return 1;   /* duplicate found */
        }
    }
    return 0;   /* all elements unique */
}

int cmpInt(const void *a, const void *b) {
    return (*(const int *)a) - (*(const int *)b);
}

/* (ii) Sort-based O(n log n) uniqueness check */
int hasDuplicateSortBased(int A[], int n, long *comparisons) {
    int *B = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) B[i] = A[i];

    qsort(B, n, sizeof(int), cmpInt);   /* O(n log n); comparisons inside
                                            qsort are not counted here since
                                            qsort doesn't expose a counter */

    *comparisons = 0;
    int found = 0;
    for (int i = 0; i + 1 < n; i++) {
        (*comparisons)++;
        if (B[i] == B[i + 1]) { found = 1; break; }
    }

    free(B);
    return found;
}

/* Fill A[0..n-1] with random numbers from a range wide enough that
   duplicates are rare (so both methods hit their WORST case, i.e. no
   early exit -- this is what makes the O(n^2) vs O(n log n) gap show
   up clearly). */
void generateUniqueLikelyArray(int A[], int n) {
    for (int i = 0; i < n; i++) {
        A[i] = rand() % (n * 100 + 1);   /* large range -> duplicates unlikely */
    }
}

int main(void) {
    srand((unsigned int) time(NULL));

    /* --- Quick correctness demo --- */
    int demoNoDup[]  = {5, 3, 8, 1, 9};
    int demoWithDup[] = {5, 3, 8, 3, 9};
    long c;
    printf("Demo (no duplicates):   result = %s\n",
           hasDuplicateNaive(demoNoDup, 5, &c) ? "DUPLICATE FOUND" : "ALL UNIQUE");
    printf("Demo (has duplicate 3): result = %s\n\n",
           hasDuplicateNaive(demoWithDup, 5, &c) ? "DUPLICATE FOUND" : "ALL UNIQUE");

    /* --- Compare naive vs sort-based comparison counts --- */
    int sizes[] = {100, 500, 1000, 2000, 4000, 8000, 16000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("n,NaiveComparisons,SortBasedComparisons(post-sort pass only)\n");
    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int *A = malloc(n * sizeof(int));
        generateUniqueLikelyArray(A, n);

        long cNaive, cSort;
        hasDuplicateNaive(A, n, &cNaive);
        hasDuplicateSortBased(A, n, &cSort);

        printf("%d,%ld,%ld\n", n, cNaive, cSort);

        free(A);
    }

    printf("\nConclusion: for large n, the naive pairwise check performs\n");
    printf("O(n^2) comparisons in the worst case (when all elements are\n");
    printf("distinct, so no early exit is possible), which becomes\n");
    printf("impractically slow. The sort-based method needs only\n");
    printf("O(n log n) time overall (sorting) plus a single O(n) linear\n");
    printf("pass, making it far more scalable for large n. In general,\n");
    printf("this is the standard trade-off in the 'element uniqueness'\n");
    printf("problem, which is known to require at least Omega(n log n)\n");
    printf("comparisons in the comparison-based model -- so sorting\n");
    printf("first is asymptotically optimal.\n");

    return 0;
}