#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int find_root(int parent[], int x) {
    if (parent[x] != x) {
        parent[x] = find_root(parent, parent[x]);
    }
    return parent[x];
}

int unite(int parent[], int size[], long long sum[], int a, int b) {
    int root_a = find_root(parent, a);
    int root_b = find_root(parent, b);

    if (root_a == root_b) return root_a;

    if (size[root_a] < size[root_b]) {
        int temp = root_a;
        root_a = root_b;
        root_b = temp;
    }

    parent[root_b] = root_a;
    size[root_a] += size[root_b];
    sum[root_a] += sum[root_b];
    return root_a;
}

int main(void) {
    int n, m;

    if (scanf("%d %d", &n, &m) != 2 || n <= 0 || m < 0) {
        fprintf(stderr, "Invalid graph size.\n");
        return 1;
    }

    long long *weight = malloc((size_t)n * sizeof(long long));
    int *head = malloc((size_t)n * sizeof(int));
    int *to = malloc((size_t)(2 * m) * sizeof(int));
    int *next = malloc((size_t)(2 * m) * sizeof(int));
    int *parent = malloc((size_t)n * sizeof(int));
    int *size = malloc((size_t)n * sizeof(int));
    long long *sum = malloc((size_t)n * sizeof(long long));
    int *order = malloc((size_t)n * sizeof(int));
    int *answer = malloc((size_t)n * sizeof(int));
    char *active = calloc((size_t)n, sizeof(char));

    if (!weight || !head || !to || !next || !parent || !size ||
        !sum || !order || !answer || !active) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(weight); free(head); free(to); free(next);
        free(parent); free(size); free(sum); free(order);
        free(answer); free(active);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (scanf("%lld", &weight[i]) != 1) {
            fprintf(stderr, "Invalid vertex weight.\n");
            return 1;
        }
        head[i] = -1;
        parent[i] = i;
        size[i] = 1;
        sum[i] = weight[i];
    }

    int edge_index = 0;
    for (int i = 0; i < m; i++) {
        int u, v;

        if (scanf("%d %d", &u, &v) != 2 ||
            u < 1 || u > n || v < 1 || v > n) {
            fprintf(stderr, "Invalid edge. Use vertices numbered 1 to n.\n");
            return 1;
        }

        u--;
        v--;

        to[edge_index] = v;
        next[edge_index] = head[u];
        head[u] = edge_index++;

        to[edge_index] = u;
        next[edge_index] = head[v];
        head[v] = edge_index++;
    }

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &order[i]) != 1 ||
            order[i] < 1 || order[i] > n) {
            fprintf(stderr, "Invalid deletion order.\n");
            return 1;
        }
        order[i]--;
    }

    long long maximum = 0;

    for (int i = n - 1; i >= 0; i--) {
        /* Current graph is the state after forward deletion i+1. */
        answer[i] = (int)maximum;

        int vertex = order[i];
        active[vertex] = 1;
        maximum = weight[vertex];

        for (int e = head[vertex]; e != -1; e = next[e]) {
            int neighbor = to[e];

            if (active[neighbor]) {
                int root = unite(parent, size, sum, vertex, neighbor);
                if (sum[root] > maximum) {
                    maximum = sum[root];
                }
            }
        }
    }

    printf("Maximum component weight after each deletion:\n");
    for (int i = 0; i < n; i++) {
        printf("%d\n", answer[i]);
    }

    free(weight); free(head); free(to); free(next);
    free(parent); free(size); free(sum); free(order);
    free(answer); free(active);
    return 0;
}