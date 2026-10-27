#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start;
    int end;
} Meeting;

int compare_meetings(const void *a, const void *b) {
    const Meeting *x = a;
    const Meeting *y = b;
    if (x->start != y->start) return (x->start > y->start) - (x->start < y->start);
    return (x->end > y->end) - (x->end < y->end);
}

void heap_push(int heap[], int *size, int end_time) {
    int i = (*size)++;
    heap[i] = end_time;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[parent] <= heap[i]) break;

        int temp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = temp;
        i = parent;
    }
}

int heap_pop(int heap[], int *size) {
    int minimum = heap[0];
    heap[0] = heap[--(*size)];

    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < *size && heap[left] < heap[smallest]) smallest = left;
        if (right < *size && heap[right] < heap[smallest]) smallest = right;
        if (smallest == i) break;

        int temp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = temp;
        i = smallest;
    }

    return minimum;
}

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n < 0) {
        fprintf(stderr, "Invalid number of meetings.\n");
        return 1;
    }

    Meeting *meetings = malloc((size_t)(n > 0 ? n : 1) * sizeof(Meeting));
    int *end_times = malloc((size_t)(n > 0 ? n : 1) * sizeof(int));

    if (meetings == NULL || end_times == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(meetings);
        free(end_times);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (scanf("%d %d", &meetings[i].start, &meetings[i].end) != 2 ||
            meetings[i].end < meetings[i].start) {
            fprintf(stderr, "Invalid meeting interval.\n");
            free(meetings);
            free(end_times);
            return 1;
        }
    }

    qsort(meetings, (size_t)n, sizeof(Meeting), compare_meetings);

    int heap_size = 0;
    int max_rooms = 0;

    for (int i = 0; i < n; i++) {
        /* Free every room whose meeting ended by this start time. */
        while (heap_size > 0 && end_times[0] <= meetings[i].start) {
            heap_pop(end_times, &heap_size);
        }

        /* Zero-duration meetings do not occupy a room. */
        if (meetings[i].start == meetings[i].end) {
            continue;
        }

        heap_push(end_times, &heap_size, meetings[i].end);

        if (heap_size > max_rooms) {
            max_rooms = heap_size;
        }
    }

    printf("Minimum rooms required: %d\n", max_rooms);

    free(meetings);
    free(end_times);
    return 0;
}