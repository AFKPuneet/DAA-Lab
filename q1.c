#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *parent;
    int *size;
    int *parity;  /* Color difference from this vertex to its parent */
} DSU;

int find(DSU *dsu, int x) {
    if (dsu->parent[x] != x) {
        int old_parent = dsu->parent[x];
        dsu->parent[x] = find(dsu, old_parent);
        dsu->parity[x] ^= dsu->parity[old_parent];
    }
    return dsu->parent[x];
}

/* Add constraint that u and v must have opposite colors. */
int add_opposite_edge(DSU *dsu, int u, int v) {
    int root_u = find(dsu, u);
    int parity_u = dsu->parity[u];

    int root_v = find(dsu, v);
    int parity_v = dsu->parity[v];

    if (root_u == root_v) {
        /* They must already have opposite colors. */
        return (parity_u ^ parity_v) == 1;
    }

    /* Merge by size; root parity must enforce opposite colors. */
    if (dsu->size[root_u] < dsu->size[root_v]) {
        dsu->parent[root_u] = root_v;
        dsu->parity[root_u] = parity_u ^ parity_v ^ 1;
        dsu->size[root_v] += dsu->size[root_u];
    } else {
        dsu->parent[root_v] = root_u;
        dsu->parity[root_v] = parity_u ^ parity_v ^ 1;
        dsu->size[root_u] += dsu->size[root_v];
    }

    return 1;
}

int main(void) {
    int vertices, edge_count;

    if (scanf("%d %d", &vertices, &edge_count) != 2 ||
        vertices <= 0 || edge_count < 0) {
        fprintf(stderr, "Invalid graph size.\n");
        return 1;
    }

    DSU dsu;
    dsu.parent = malloc((size_t)vertices * sizeof(int));
    dsu.size = malloc((size_t)vertices * sizeof(int));
    dsu.parity = calloc((size_t)vertices, sizeof(int));

    if (!dsu.parent || !dsu.size || !dsu.parity) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(dsu.parent);
        free(dsu.size);
        free(dsu.parity);
        return 1;
    }

    for (int i = 0; i < vertices; i++) {
        dsu.parent[i] = i;
        dsu.size[i] = 1;
    }

    for (int i = 0; i < edge_count; i++) {
        int u, v;

        if (scanf("%d %d", &u, &v) != 2 ||
            u < 1 || u > vertices || v < 1 || v > vertices) {
            fprintf(stderr, "Invalid edge. Use vertices numbered 1 to V.\n");
            free(dsu.parent);
            free(dsu.size);
            free(dsu.parity);
            return 1;
        }

        /* Convert to zero-based indices. */
        u--;
        v--;

        if (add_opposite_edge(&dsu, u, v)) {
            printf("Edge (%d, %d): accepted\n", u + 1, v + 1);
        } else {
            printf("Edge (%d, %d): rejected (would violate bipartiteness)\n",
                   u + 1, v + 1);
        }
    }

    free(dsu.parent);
    free(dsu.size);
    free(dsu.parity);
    return 0;
}