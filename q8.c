#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n <= 0) {
        return 1;
    }

    long long *price = malloc(((size_t)n + 1) * sizeof(long long));
    long long *revenue = malloc(((size_t)n + 1) * sizeof(long long));
    int *firstCut = malloc(((size_t)n + 1) * sizeof(int));

    if (price == NULL || revenue == NULL || firstCut == NULL) {
        free(price);
        free(revenue);
        free(firstCut);
        return 1;
    }

    price[0] = 0;
    for (int length = 1; length <= n; length++) {
        if (scanf("%lld", &price[length]) != 1) {
            free(price);
            free(revenue);
            free(firstCut);
            return 1;
        }
    }

    revenue[0] = 0;
    firstCut[0] = 0;

    for (int length = 1; length <= n; length++) {
        revenue[length] = LLONG_MIN;

        for (int piece = 1; piece <= length; piece++) {
            long long candidate = price[piece] + revenue[length - piece];

            if (candidate > revenue[length]) {
                revenue[length] = candidate;
                firstCut[length] = piece;
            }
        }
    }

    printf("Maximum revenue: %lld\n", revenue[n]);
    printf("Piece lengths:");

    for (int remaining = n; remaining > 0; remaining -= firstCut[remaining]) {
        printf(" %d", firstCut[remaining]);
    }
    printf("\n");

    free(price);
    free(revenue);
    free(firstCut);
    return 0;
}