#include <stdio.h>
#include <limits.h>

int main() {
    int n;
    scanf("%d", &n);

    int p[n + 1];

    for (int i = 0; i <= n; i++)
        scanf("%d", &p[i]);

    long long dp[n + 1][n + 1];
    int split[n + 1][n + 1];

    for (int i = 1; i <= n; i++)
        dp[i][i] = 0;

    for (int len = 2; len <= n; len++) {
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;
            dp[i][j] = LLONG_MAX;

            for (int k = i; k < j; k++) {
                long long cost = dp[i][k] +
                                 dp[k + 1][j] +
                                 (long long)p[i - 1] * p[k] * p[j];

                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    split[i][j] = k;
                }
            }
        }
    }

    void printOrder(int i, int j) {
        if (i == j) {
            printf("A%d", i);
            return;
        }

        printf("(");
        printOrder(i, split[i][j]);
        printOrder(split[i][j] + 1, j);
        printf(")");
    }

    printf("%lld\n", dp[1][n]);
    printOrder(1, n);
    printf("\n");

    return 0;
}