#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int find_root(int parent[], int x) {
    if (parent[x] != x) {
        parent[x] = find_root(parent, parent[x]);
    }
    return parent[x];
}

void unite(int parent[], int size[], long long sum[], int a, int b) {
    int root_a = find_root(parent, a);
    int root_b = find_root(parent, b);

    if (root_a == root_b) return;

    if (size[root_a] < size[root_b]) {
        int temp = root_a;
        root_a = root_b;
        root_b = temp;
    }

    parent[root_b] = root_a;
    size[root_a] += size[root_b];
    sum[root_a] += sum[root_b];
}

void report_maximum(int n, int parent[], int size[],
                    long long sum[], int limit) {
    int found = 0;
    long long maximum = LLONG_MIN;

    for (int i = 0; i < n; i++) {
        if (find_root(parent, i) == i && size[i] <= limit) {
            if (!found || sum[i] > maximum) {
                maximum = sum[i];
                found = 1;
            }
        }
    }

    if (found) {
        printf("%lld\n", maximum);
    } else {
        printf("No component has size at most K\n");
    }
}

int main(void) {
    int n, m, limit;

    if (scanf("%d %d %d", &n, &m, &limit) != 3 ||
        n <= 0 || m < 0 || limit < 0) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }

    int *parent = malloc((size_t)n * sizeof(int));
    int *size = malloc((size_t)n * sizeof(int));
    long long *sum = malloc((size_t)n * sizeof(long long));

    if (parent == NULL || size == NULL || sum == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(parent);
        free(size);
        free(sum);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &sum[i]) != 1) {
            fprintf(stderr, "Invalid vertex weight.\n");
            free(parent);
            free(size);
            free(sum);
            return 1;
        }
        parent[i] = i;
        size[i] = 1;
    }

    printf("Initial maximum: ");
    report_maximum(n, parent, size, sum, limit);

    for (int i = 0; i < m; i++) {
        int u, v;

        if (scanf("%d %d", &u, &v) != 2 ||
            u < 1 || u > n || v < 1 || v > n) {
            fprintf(stderr, "Invalid edge. Use vertices numbered 1 to n.\n");
            free(parent);
            free(size);
            free(sum);
            return 1;
        }

        unite(parent, size, sum, u - 1, v - 1);

        printf("After edge %d: ", i + 1);
        report_maximum(n, parent, size, sum, limit);
    }

    free(parent);
    free(size);
    free(sum);
    return 0;
}