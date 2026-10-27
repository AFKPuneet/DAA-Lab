#include <stdio.h>
#include <stdlib.h>

void heap_push(long long heap[], int *size, long long value) {
    int i = (*size)++;
    heap[i] = value;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[parent] <= heap[i]) break;

        long long temp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = temp;
        i = parent;
    }
}

long long heap_pop(long long heap[], int *size) {
    long long minimum = heap[0];
    heap[0] = heap[--(*size)];

    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < *size && heap[left] < heap[smallest]) smallest = left;
        if (right < *size && heap[right] < heap[smallest]) smallest = right;
        if (smallest == i) break;

        long long temp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = temp;
        i = smallest;
    }

    return minimum;
}

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n < 0) {
        fprintf(stderr, "Invalid number of sticks.\n");
        return 1;
    }

    long long *heap = malloc((size_t)(n > 0 ? n : 1) * sizeof(long long));
    if (heap == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    int size = 0;

    for (int i = 0; i < n; i++) {
        long long length;
        if (scanf("%lld", &length) != 1 || length < 0) {
            fprintf(stderr, "Invalid stick length.\n");
            free(heap);
            return 1;
        }
        heap_push(heap, &size, length);
    }

    long long total_cost = 0;

    while (size > 1) {
        long long first = heap_pop(heap, &size);
        long long second = heap_pop(heap, &size);
        long long combined = first + second;

        total_cost += combined;
        heap_push(heap, &size, combined);
    }

    printf("Minimum total cost: %lld\n", total_cost);

    free(heap);
    return 0;
}