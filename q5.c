#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid number of children.\n");
        return 1;
    }

    int *rating = malloc((size_t)n * sizeof(int));
    long long *candies = malloc((size_t)n * sizeof(long long));

    if (rating == NULL || candies == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(rating);
        free(candies);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &rating[i]) != 1) {
            fprintf(stderr, "Invalid rating.\n");
            free(rating);
            free(candies);
            return 1;
        }
        candies[i] = 1;
    }

    /* Ensure each higher-rated child gets more than the left neighbour. */
    for (int i = 1; i < n; i++) {
        if (rating[i] > rating[i - 1]) {
            candies[i] = candies[i - 1] + 1;
        }
    }

    /* Ensure each higher-rated child gets more than the right neighbour. */
    for (int i = n - 2; i >= 0; i--) {
        if (rating[i] > rating[i + 1] &&
            candies[i] <= candies[i + 1]) {
            candies[i] = candies[i + 1] + 1;
        }
    }

    long long total = 0;
    for (int i = 0; i < n; i++) {
        total += candies[i];
    }

    printf("Minimum candies: %lld\n", total);
    printf("Candy allocation:");
    for (int i = 0; i < n; i++) {
        printf(" %lld", candies[i]);
    }
    printf("\n");

    free(rating);
    free(candies);
    return 0;
}