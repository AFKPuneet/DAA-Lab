#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n <= 0) {
        return 1;
    }

    long long *a = malloc((size_t)n * sizeof(long long));
    long long *dp = malloc((size_t)n * sizeof(long long));

    if (a == NULL || dp == NULL) {
        free(a);
        free(dp);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &a[i]) != 1) {
            free(a);
            free(dp);
            return 1;
        }
    }

    long long best = LLONG_MIN;

    for (int i = 0; i < n; i++) {
        dp[i] = a[i];

        for (int j = 0; j < i; j++) {
            if (a[j] < a[i] && dp[j] + a[i] > dp[i]) {
                dp[i] = dp[j] + a[i];
            }
        }

        if (dp[i] > best) {
            best = dp[i];
        }
    }

    printf("Maximum sum: %lld\n", best);

    free(a);
    free(dp);
    return 0;
}