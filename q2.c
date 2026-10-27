#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

int find_root(int parent[], int x) {
    if (parent[x] != x) {
        parent[x] = find_root(parent, parent[x]);
    }
    return parent[x];
}

int unite(int parent[], int size[], int a, int b) {
    int root_a = find_root(parent, a);
    int root_b = find_root(parent, b);

    if (root_a == root_b) return 0;

    if (size[root_a] < size[root_b]) {
        int temp = root_a;
        root_a = root_b;
        root_b = temp;
    }

    parent[root_b] = root_a;
    size[root_a] += size[root_b];
    return 1;
}

int main(void) {
    int rows, cols, operations;

    if (scanf("%d %d %d", &rows, &cols, &operations) != 3 ||
        rows <= 0 || cols <= 0 || operations < 0) {
        fprintf(stderr, "Invalid grid dimensions or operation count.\n");
        return 1;
    }

    if ((size_t)rows > SIZE_MAX / (size_t)cols) {
        fprintf(stderr, "Grid is too large.\n");
        return 1;
    }

    size_t cells = (size_t)rows * (size_t)cols;
    int *parent = malloc(cells * sizeof(int));
    int *size = malloc(cells * sizeof(int));

    if (parent == NULL || size == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(parent);
        free(size);
        return 1;
    }

    /* parent == -1 means the cell is still water. */
    for (size_t i = 0; i < cells; i++) {
        parent[i] = -1;
        size[i] = 0;
    }

    int islands = 0;

    for (int op = 0; op < operations; op++) {
        int r, c;

        if (scanf("%d %d", &r, &c) != 2 ||
            r < 1 || r > rows || c < 1 || c > cols) {
            fprintf(stderr, "Invalid cell. Use 1-based row and column numbers.\n");
            free(parent);
            free(size);
            return 1;
        }

        r--;
        c--;

        int cell = r * cols + c;

        if (parent[cell] == -1) {
            parent[cell] = cell;
            size[cell] = 1;
            islands++;

            const int dr[] = {-1, 1, 0, 0};
            const int dc[] = {0, 0, -1, 1};

            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];

                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                    int neighbor = nr * cols + nc;
                    if (parent[neighbor] != -1 &&
                        unite(parent, size, cell, neighbor)) {
                        islands--;
                    }
                }
            }
        }

        printf("%d\n", islands);
    }

    free(parent);
    free(size);
    return 0;
}