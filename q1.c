#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Edge {
    int to;
    int weight;
    struct Edge *next;
} Edge;

typedef struct QueueNode {
    int state;
    long long distance;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *back;
} Deque;

int push_front(Deque *deque, int state, long long distance) {
    QueueNode *node = malloc(sizeof(QueueNode));
    if (node == NULL) return 0;

    node->state = state;
    node->distance = distance;
    node->next = deque->front;
    deque->front = node;
    if (deque->back == NULL) deque->back = node;
    return 1;
}

int push_back(Deque *deque, int state, long long distance) {
    QueueNode *node = malloc(sizeof(QueueNode));
    if (node == NULL) return 0;

    node->state = state;
    node->distance = distance;
    node->next = NULL;

    if (deque->back == NULL) {
        deque->front = deque->back = node;
    } else {
        deque->back->next = node;
        deque->back = node;
    }
    return 1;
}

int pop_front(Deque *deque, int *state, long long *distance) {
    if (deque->front == NULL) return 0;

    QueueNode *node = deque->front;
    *state = node->state;
    *distance = node->distance;

    deque->front = node->next;
    if (deque->front == NULL) deque->back = NULL;
    free(node);
    return 1;
}

void free_deque(Deque *deque) {
    int state;
    long long distance;
    while (pop_front(deque, &state, &distance)) {}
}

int main(void) {
    int n, m, source;
    long long penalty;

    if (scanf("%d %d %d %lld", &n, &m, &source, &penalty) != 4 ||
        n <= 0 || m < 0 || source < 1 || source > n || penalty < 0) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }

    Edge **graph = calloc((size_t)n, sizeof(Edge *));
    long long *dist = malloc((size_t)(2 * n) * sizeof(long long));

    if (graph == NULL || dist == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(graph);
        free(dist);
        return 1;
    }

    for (int i = 0; i < m; i++) {
        int u, v, w;

        if (scanf("%d %d %d", &u, &v, &w) != 3 ||
            u < 1 || u > n || v < 1 || v > n || (w != 0 && w != 1)) {
            fprintf(stderr, "Invalid directed edge.\n");
            return 1;
        }

        Edge *edge = malloc(sizeof(Edge));
        if (edge == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            return 1;
        }
        edge->to = v - 1;
        edge->weight = w;
        edge->next = graph[u - 1];
        graph[u - 1] = edge;
    }

    for (int i = 0; i < 2 * n; i++) dist[i] = LLONG_MAX;

    Deque deque = {NULL, NULL};
    int start_state = 2 * (source - 1); /* even parity */
    dist[start_state] = 0;

    if (!push_front(&deque, start_state, 0)) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    int state;
    long long queued_distance;

    while (pop_front(&deque, &state, &queued_distance)) {
        if (queued_distance != dist[state]) continue;

        int u = state / 2;
        int parity = state % 2;

        for (Edge *edge = graph[u]; edge != NULL; edge = edge->next) {
            int next_parity = parity ^ edge->weight;
            int next_state = 2 * edge->to + next_parity;
            long long candidate = dist[state] + edge->weight;

            if (candidate < dist[next_state]) {
                dist[next_state] = candidate;

                int ok = edge->weight == 0
                    ? push_front(&deque, next_state, candidate)
                    : push_back(&deque, next_state, candidate);

                if (!ok) {
                    fprintf(stderr, "Memory allocation failed.\n");
                    free_deque(&deque);
                    return 1;
                }
            }
        }
    }

    printf("Minimum effective costs:\n");
    for (int v = 0; v < n; v++) {
        long long even = dist[2 * v];
        long long odd = dist[2 * v + 1];

        if (odd != LLONG_MAX && odd <= LLONG_MAX - penalty) {
            odd += penalty;
        }

        long long answer = even < odd ? even : odd;
        if (answer == LLONG_MAX) {
            printf("Vertex %d: unreachable\n", v + 1);
        } else {
            printf("Vertex %d: %lld\n", v + 1, answer);
        }
    }

    free_deque(&deque);

    for (int i = 0; i < n; i++) {
        Edge *edge = graph[i];
        while (edge != NULL) {
            Edge *next = edge->next;
            free(edge);
            edge = next;
        }
    }

    free(graph);
    free(dist);
    return 0;
}