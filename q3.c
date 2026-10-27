#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char X[1001], Y[1001];

    if (scanf("%1000s %1000s", X, Y) != 2) {
        return 1;
    }

    int m = (int)strlen(X);
    int n = (int)strlen(Y);

    int **dp = malloc((size_t)(m + 1) * sizeof(int *));
    if (dp == NULL) return 1;

    for (int i = 0; i <= m; i++) {
        dp[i] = calloc((size_t)(n + 1), sizeof(int));
        if (dp[i] == NULL) {
            for (int k = 0; k < i; k++) free(dp[k]);
            free(dp);
            return 1;
        }
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else if (dp[i - 1][j] >= dp[i][j - 1]) {
                dp[i][j] = dp[i - 1][j];
            } else {
                dp[i][j] = dp[i][j - 1];
            }
        }
    }

    int length = dp[m][n];
    char *lcs = malloc((size_t)length + 1);
    if (lcs == NULL) {
        for (int i = 0; i <= m; i++) free(dp[i]);
        free(dp);
        return 1;
    }

    lcs[length] = '\0';

    int i = m, j = n, pos = length;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs[--pos] = X[i - 1];
            i--;
            j--;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    printf("LCS length: %d\n", length);
    printf("LCS: %s\n", lcs);

    free(lcs);
    for (int k = 0; k <= m; k++) free(dp[k]);
    free(dp);
    return 0;
}