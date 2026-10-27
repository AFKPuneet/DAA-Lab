#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

    int *dp = malloc(((size_t)amount + 1) * sizeof(int));
    if (dp == NULL) {
        free(coins);
        return 1;
    }

    for (int x = 0; x <= amount; x++) {
        dp[x] = INT_MAX;
    }
    dp[0] = 0;

    for (int x = 1; x <= amount; x++) {
        for (int i = 0; i < n; i++) {
            if (coins[i] <= x && dp[x - coins[i]] != INT_MAX) {
                int candidate = dp[x - coins[i]] + 1;
                if (candidate < dp[x]) {
                    dp[x] = candidate;
                }
            }
        }
    }

    if (dp[amount] == INT_MAX) {
        printf("-1\n");
    } else {
        printf("%d\n", dp[amount]);
    }

    free(dp);
    free(coins);
    return 0;
}