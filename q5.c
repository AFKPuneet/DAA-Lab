#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n <= 0) {
        return 1;
    }

    int *a = malloc((size_t)n * sizeof(int));
    int *dp = malloc((size_t)n * sizeof(int));

    if (a == NULL || dp == NULL) {
        free(a);
        free(dp);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &a[i]) != 1) {
            free(a);
            free(dp);
            return 1;
        }
    }

    int longest = 1;

    for (int i = 0; i < n; i++) {
        dp[i] = 1;

        for (int j = 0; j < i; j++) {
            if (a[j] < a[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
        }

        if (dp[i] > longest) {
            longest = dp[i];
        }
    }

    printf("LIS length: %d\n", longest);

    free(a);
    free(dp);
    return 0;
}