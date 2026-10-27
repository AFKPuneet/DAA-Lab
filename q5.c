#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int root;
    int value;
} ComponentValue;

int find_root(int parent[], int x) {
    if (parent[x] != x) {
        parent[x] = find_root(parent, parent[x]);
    }
    return parent[x];
}

void unite(int parent[], int size[], int a, int b) {
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
}

int compare_component_values(const void *a, const void *b) {
    const ComponentValue *x = a;
    const ComponentValue *y = b;

    if (x->root != y->root) {
        return (x->root > y->root) - (x->root < y->root);
    }
    return (x->value > y->value) - (x->value < y->value);
}

int main(void) {
    int n, m;

    if (scanf("%d %d", &n, &m) != 2 || n <= 0 || m < 0) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }

    int *values = malloc((size_t)n * sizeof(int));
    int *parent = malloc((size_t)n * sizeof(int));
    int *size = malloc((size_t)n * sizeof(int));
    int *answer = malloc((size_t)n * sizeof(int));
    int *cursor = malloc((size_t)n * sizeof(int));
    ComponentValue *component_values =
        malloc((size_t)n * sizeof(ComponentValue));

    if (!values || !parent || !size || !answer || !cursor ||
        !component_values) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(values); free(parent); free(size);
        free(answer); free(cursor); free(component_values);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &values[i]) != 1 ||
            values[i] < 1 || values[i] > n) {
            fprintf(stderr, "Enter a permutation of 1 through n.\n");
            return 1;
        }
        parent[i] = i;
        size[i] = 1;
    }

    for (int i = 0; i < m; i++) {
        int u, v;

        if (scanf("%d %d", &u, &v) != 2 ||
            u < 1 || u > n || v < 1 || v > n) {
            fprintf(stderr, "Invalid edge. Use vertices numbered 1 to n.\n");
            return 1;
        }

        unite(parent, size, u - 1, v - 1);
    }

    for (int i = 0; i < n; i++) {
        component_values[i].root = find_root(parent, i);
        component_values[i].value = values[i];
    }

    qsort(component_values, (size_t)n, sizeof(ComponentValue),
          compare_component_values);

    for (int i = 0; i < n; i++) {
        cursor[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        int root = component_values[i].root;
        if (cursor[root] == -1) {
            cursor[root] = i;
        }
    }

    /* Visit indices in ascending order and assign each component's
       values from smallest to largest. */
    for (int i = 0; i < n; i++) {
        int root = find_root(parent, i);
        answer[i] = component_values[cursor[root]++].value;
    }

    printf("Lexicographically smallest permutation:\n");
    for (int i = 0; i < n; i++) {
        printf("%d%c", answer[i], i + 1 == n ? '\n' : ' ');
    }

    free(values);
    free(parent);
    free(size);
    free(answer);
    free(cursor);
    free(component_values);
    return 0;
}