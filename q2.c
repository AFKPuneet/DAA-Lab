#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n, amount;

    if (scanf("%d", &n) != 1 || n <= 0) {
        return 1;
    }

    int *coins = malloc((size_t)n * sizeof(int));
    if (coins == NULL) {
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &coins[i]) != 1 || coins[i] <= 0) {
            free(coins);
            return 1;
        }
    }

    if (scanf("%d", &amount) != 1 || amount < 0) {
        free(coins);
        return 1;
    }

    unsigned long long *ways =
        calloc((size_t)amount + 1, sizeof(unsigned long long));

    if (ways == NULL) {
        free(coins);
        return 1;
    }

    ways[0] = 1;

    for (int i = 0; i < n; i++) {
        for (int value = coins[i]; value <= amount; value++) {
            ways[value] += ways[value - coins[i]];
        }
    }

    printf("%llu\n", ways[amount]);

    free(ways);
    free(coins);
    return 0;
}