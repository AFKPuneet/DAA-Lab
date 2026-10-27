#include <stdio.h>
#include <stdlib.h>

typedef struct Edge {
    int to;
    struct Edge *next;
} Edge;

int main(void) {
    int n, m, k, radius;

    if (scanf("%d %d %d %d", &n, &m, &k, &radius) != 4 ||
        n <= 0 || m < 0 || k < 0 || k > n || radius < 0) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }

    Edge **graph = calloc((size_t)n, sizeof(Edge *));
    int *distance = malloc((size_t)n * sizeof(int));
    int *owner = malloc((size_t)n * sizeof(int));
    int *queue = malloc((size_t)n * sizeof(int));
    int *hubs = malloc((size_t)(k > 0 ? k : 1) * sizeof(int));

    if (!graph || !distance || !owner || !queue || !hubs) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(graph); free(distance); free(owner); free(queue); free(hubs);
        return 1;
    }

    for (int i = 0; i < m; i++) {
        int u, v;

        if (scanf("%d %d", &u, &v) != 2 ||
            u < 1 || u > n || v < 1 || v > n) {
            fprintf(stderr, "Invalid edge.\n");
            return 1;
        }

        u--;
        v--;

        Edge *uv = malloc(sizeof(Edge));
        Edge *vu = malloc(sizeof(Edge));
        if (!uv || !vu) {
            fprintf(stderr, "Memory allocation failed.\n");
            free(uv); free(vu);
            return 1;
        }

        uv->to = v;
        uv->next = graph[u];
        graph[u] = uv;

        vu->to = u;
        vu->next = graph[v];
        graph[v] = vu;
    }

    for (int i = 0; i < k; i++) {
        if (scanf("%d", &hubs[i]) != 1 ||
            hubs[i] < 1 || hubs[i] > n) {
            fprintf(stderr, "Invalid hub vertex.\n");
            return 1;
        }
        hubs[i]--;
    }

    for (int v = 0; v < n; v++) {
        distance[v] = -1;
        owner[v] = -1;
    }

    int front = 0, back = 0;

    /* Hub input order defines hub indices 1 through K. */
    for (int i = 0; i < k; i++) {
        int hub = hubs[i];
        if (distance[hub] == -1) {
            distance[hub] = 0;
            owner[hub] = i + 1;
            queue[back++] = hub;
        }
    }

    while (front < back) {
        int u = queue[front++];

        for (Edge *edge = graph[u]; edge != NULL; edge = edge->next) {
            int v = edge->to;

            if (distance[v] == -1) {
                distance[v] = distance[u] + 1;
                owner[v] = owner[u];
                queue[back++] = v;
            }
        }
    }

    printf("Vertex  Owner  Distance\n");
    for (int v = 0; v < n; v++) {
        if (distance[v] == -1 || distance[v] > radius) {
            printf("%d       -1     -1\n", v + 1);
        } else {
            printf("%d       %d      %d\n",
                   v + 1, owner[v], distance[v]);
        }
    }

    for (int v = 0; v < n; v++) {
        Edge *edge = graph[v];
        while (edge != NULL) {
            Edge *next = edge->next;
            free(edge);
            edge = next;
        }
    }

    free(graph);
    free(distance);
    free(owner);
    free(queue);
    free(hubs);
    return 0;
}