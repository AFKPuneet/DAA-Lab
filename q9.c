#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define IDX(i, j, n) ((size_t)(i) * (n) + (j))

void print_tree(int i, int j, int n, int *root) {
    if (i == j) {
        printf("%d", i + 1);
        return;
    }

    int k = root[IDX(i, j, n)];
    printf("(");
    print_tree(i, k, n, root);
    printf(",");
    print_tree(k + 1, j, n, root);
    printf(")");
}

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid number of weights.\n");
        return 1;
    }

    long long *weight = malloc((size_t)n * sizeof(long long));
    size_t cells = (size_t)n * n;
    long long *prefix = malloc(((size_t)n + 1) * sizeof(long long));
    long long *cost = malloc(cells * sizeof(long long));
    int *root = malloc(cells * sizeof(int));

    if (weight == NULL || prefix == NULL || cost == NULL || root == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(weight); free(prefix); free(cost); free(root);
        return 1;
    }

    prefix[0] = 0;
    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &weight[i]) != 1 || weight[i] < 0) {
            fprintf(stderr, "Weights must be nonnegative.\n");
            free(weight); free(prefix); free(cost); free(root);
            return 1;
        }
        prefix[i + 1] = prefix[i] + weight[i];
        cost[IDX(i, i, n)] = 0;  /* One leaf has depth zero. */
        root[IDX(i, i, n)] = -1;
    }

    /* Solve each interval of leaves, from shorter intervals to longer ones. */
    for (int length = 2; length <= n; length++) {
        for (int i = 0; i + length <= n; i++) {
            int j = i + length - 1;
            long long interval_weight = prefix[j + 1] - prefix[i];
            long long best = LLONG_MAX;
            int best_split = -1;

            for (int k = i; k < j; k++) {
                long long candidate =
                    cost[IDX(i, k, n)] +
                    cost[IDX(k + 1, j, n)] +
                    interval_weight;

                if (candidate < best) {
                    best = candidate;
                    best_split = k;
                }
            }

            cost[IDX(i, j, n)] = best;
            root[IDX(i, j, n)] = best_split;
        }
    }

    printf("Minimum weighted path length: %lld\n", cost[IDX(0, n - 1, n)]);
    printf("Alphabetic tree (leaves labeled by input position): ");
    print_tree(0, n - 1, n, root);
    printf("\n");

    free(weight);
    free(prefix);
    free(cost);
    free(root);
    return 0;
}