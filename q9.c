#include <stdio.h>
#include <stdlib.h>

#define IDX(i, j, width) ((size_t)(i) * (width) + (j))

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n <= 0) {
        return 1;
    }

    double *p = malloc(((size_t)n + 1) * sizeof(double));
    double *q = malloc(((size_t)n + 1) * sizeof(double));
    size_t cells = ((size_t)n + 2) * ((size_t)n + 1);
    double *cost = calloc(cells, sizeof(double));
    double *weight = calloc(cells, sizeof(double));

    if (p == NULL || q == NULL || cost == NULL || weight == NULL) {
        free(p);
        free(q);
        free(cost);
        free(weight);
        return 1;
    }

    /* p[1..n]: successful-search probabilities */
    for (int i = 1; i <= n; i++) {
        if (scanf("%lf", &p[i]) != 1) {
            free(p); free(q); free(cost); free(weight);
            return 1;
        }
    }

    /* q[0..n]: unsuccessful-search probabilities */
    for (int i = 0; i <= n; i++) {
        if (scanf("%lf", &q[i]) != 1) {
            free(p); free(q); free(cost); free(weight);
            return 1;
        }
    }

    /*
     * cost[i][j] is the optimal cost for keys i through j.
     * When i > j, the subtree is a dummy key with cost q[i-1].
     */
    for (int i = 1; i <= n + 1; i++) {
        cost[IDX(i, i - 1, n + 1)] = q[i - 1];
        weight[IDX(i, i - 1, n + 1)] = q[i - 1];
    }

    for (int length = 1; length <= n; length++) {
        for (int i = 1; i <= n - length + 1; i++) {
            int j = i + length - 1;

            weight[IDX(i, j, n + 1)] =
                weight[IDX(i, j - 1, n + 1)] + p[j] + q[j];

            cost[IDX(i, j, n + 1)] = 1e100;

            for (int root = i; root <= j; root++) {
                double candidate =
                    cost[IDX(i, root - 1, n + 1)] +
                    cost[IDX(root + 1, j, n + 1)] +
                    weight[IDX(i, j, n + 1)];

                if (candidate < cost[IDX(i, j, n + 1)]) {
                    cost[IDX(i, j, n + 1)] = candidate;
                }
            }
        }
    }

    printf("Minimum expected search cost: %.6f\n",
           cost[IDX(1, n, n + 1)]);

    free(p);
    free(q);
    free(cost);
    free(weight);
    return 0;
}