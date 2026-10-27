#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    long long value;
    int index;
} HeapNode;

void swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_push(HeapNode heap[], int *size, HeapNode node) {
    int i = (*size)++;
    heap[i] = node;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[parent].value >= heap[i].value) break;
        swap(&heap[parent], &heap[i]);
        i = parent;
    }
}

HeapNode heap_pop(HeapNode heap[], int *size) {
    HeapNode maximum = heap[0];
    heap[0] = heap[--(*size)];

    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < *size && heap[left].value > heap[largest].value)
            largest = left;
        if (right < *size && heap[right].value > heap[largest].value)
            largest = right;
        if (largest == i) break;

        swap(&heap[i], &heap[largest]);
        i = largest;
    }

    return maximum;
}

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid array size.\n");
        return 1;
    }

    long long *current = malloc((size_t)n * sizeof(long long));
    long long *best = malloc((size_t)n * sizeof(long long));
    HeapNode *heap = malloc((size_t)n * sizeof(HeapNode));

    if (current == NULL || best == NULL || heap == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(current);
        free(best);
        free(heap);
        return 1;
    }

    int heap_size = 0;
    long long minimum = LLONG_MAX;

    for (int i = 0; i < n; i++) {
        long long value;

        if (scanf("%lld", &value) != 1 || value <= 0) {
            fprintf(stderr, "Values must be positive integers.\n");
            free(current);
            free(best);
            free(heap);
            return 1;
        }

        if (value % 2 != 0) {
            if (value > LLONG_MAX / 2) {
                fprintf(stderr, "Value too large to double safely.\n");
                free(current);
                free(best);
                free(heap);
                return 1;
            }
            value *= 2;
        }

        current[i] = value;
        if (value < minimum) minimum = value;

        HeapNode node = {value, i};
        heap_push(heap, &heap_size, node);
    }

    long long smallest_deviation = LLONG_MAX;

    while (heap_size > 0) {
        HeapNode maximum_node = heap_pop(heap, &heap_size);
        long long maximum = maximum_node.value;
        long long deviation = maximum - minimum;

        if (deviation < smallest_deviation) {
            smallest_deviation = deviation;
            for (int i = 0; i < n; i++) {
                best[i] = current[i];
            }
        }

        /* The maximum cannot be reduced further if it is odd. */
        if (maximum % 2 != 0) break;

        long long reduced = maximum / 2;
        current[maximum_node.index] = reduced;
        if (reduced < minimum) minimum = reduced;

        HeapNode node = {reduced, maximum_node.index};
        heap_push(heap, &heap_size, node);
    }

    printf("Minimum deviation: %lld\n", smallest_deviation);
    printf("Array achieving it:");
    for (int i = 0; i < n; i++) {
        printf(" %lld", best[i]);
    }
    printf("\n");

    free(current);
    free(best);
    free(heap);
    return 0;
}