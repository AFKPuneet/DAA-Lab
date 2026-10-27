#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char A[1001], B[1001];

    if (scanf("%1000s %1000s", A, B) != 2) {
        return 1;
    }

    int m = (int)strlen(A);
    int n = (int)strlen(B);

    int **dp = malloc((size_t)(m + 1) * sizeof(int *));
    if (dp == NULL) return 1;

    for (int i = 0; i <= m; i++) {
        dp[i] = malloc((size_t)(n + 1) * sizeof(int));
        if (dp[i] == NULL) {
            for (int k = 0; k < i; k++) free(dp[k]);
            free(dp);
            return 1;
        }
    }

    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int cost = (A[i - 1] == B[j - 1]) ? 0 : 1;
            int deletion = dp[i - 1][j] + 1;
            int insertion = dp[i][j - 1] + 1;
            int substitution = dp[i - 1][j - 1] + cost;

            dp[i][j] = deletion;
            if (insertion < dp[i][j]) dp[i][j] = insertion;
            if (substitution < dp[i][j]) dp[i][j] = substitution;
        }
    }

    printf("Edit distance: %d\n", dp[m][n]);
    printf("Edits (from A to B):\n");

    int i = m, j = n;

    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 &&
            A[i - 1] == B[j - 1] &&
            dp[i][j] == dp[i - 1][j - 1]) {
            i--;
            j--;
        } else if (i > 0 && j > 0 &&
                   dp[i][j] == dp[i - 1][j - 1] + 1) {
            printf("Substitute '%c' with '%c'\n", A[i - 1], B[j - 1]);
            i--;
            j--;
        } else if (i > 0 && dp[i][j] == dp[i - 1][j] + 1) {
            printf("Delete '%c'\n", A[i - 1]);
            i--;
        } else {
            printf("Insert '%c'\n", B[j - 1]);
            j--;
        }
    }

    for (int k = 0; k <= m; k++) free(dp[k]);
    free(dp);
    return 0;
}