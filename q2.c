#include <stdio.h>
#include <limits.h>

int main() {
    int E, F;
    scanf("%d %d", &E, &F);

    int dp[E + 1][F + 1];

    for (int e = 1; e <= E; e++) {
        dp[e][0] = 0;
        dp[e][1] = 1;
    }

    for (int f = 0; f <= F; f++)
        dp[1][f] = f;

    for (int e = 2; e <= E; e++) {
        for (int f = 2; f <= F; f++) {
            dp[e][f] = INT_MAX;

            for (int x = 1; x <= f; x++) {
                int attempts = 1;
                int broken = dp[e - 1][x - 1];
                int survived = dp[e][f - x];
                int worst = attempts + (broken > survived ? broken : survived);

                if (worst < dp[e][f])
                    dp[e][f] = worst;
            }
        }
    }

    printf("%d\n", dp[E][F]);

    return 0;
}