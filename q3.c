#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long long distance;
    long long fuel;
} Station;

void swap(long long *a, long long *b) {
    long long temp = *a;
    *a = *b;
    *b = temp;
}

void heap_push(long long heap[], int *size, long long fuel) {
    int i = (*size)++;
    heap[i] = fuel;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[parent] >= heap[i]) break;
        swap(&heap[parent], &heap[i]);
        i = parent;
    }
}

long long heap_pop(long long heap[], int *size) {
    long long maximum = heap[0];
    heap[0] = heap[--(*size)];

    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < *size && heap[left] > heap[largest]) largest = left;
        if (right < *size && heap[right] > heap[largest]) largest = right;
        if (largest == i) break;

        swap(&heap[i], &heap[largest]);
        i = largest;
    }

    return maximum;
}

int compare_stations(const void *a, const void *b) {
    const Station *x = a;
    const Station *y = b;
    return (x->distance > y->distance) - (x->distance < y->distance);
}

int main(void) {
    int n;
    long long target, fuel;

    /* Input: target distance, starting fuel, station count */
    if (scanf("%lld %lld %d", &target, &fuel, &n) != 3 ||
        target < 0 || fuel < 0 || n < 0) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }

    Station *stations = malloc((size_t)(n + 1) * sizeof(Station));
    long long *heap = malloc((size_t)(n + 1) * sizeof(long long));

    if (stations == NULL || heap == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(stations);
        free(heap);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (scanf("%lld %lld", &stations[i].distance,
                  &stations[i].fuel) != 2 ||
            stations[i].distance < 0 || stations[i].fuel < 0) {
            fprintf(stderr, "Invalid station data.\n");
            free(stations);
            free(heap);
            return 1;
        }
    }

    /* The target acts as a final station with no fuel. */
    stations[n].distance = target;
    stations[n].fuel = 0;
    qsort(stations, (size_t)(n + 1), sizeof(Station), compare_stations);

    int heap_size = 0;
    int stops = 0;
    long long previous = 0;
    int possible = 1;

    for (int i = 0; i <= n; i++) {
        long long travel = stations[i].distance - previous;

        while (fuel < travel) {
            if (heap_size == 0) {
                possible = 0;
                break;
            }
            fuel += heap_pop(heap, &heap_size);
            stops++;
        }

        if (!possible) break;

        fuel -= travel;
        previous = stations[i].distance;

        /* Make this station's fuel available for future travel. */
        if (i < n) {
            heap_push(heap, &heap_size, stations[i].fuel);
        }
    }

    if (possible) {
        printf("Minimum refuelling stops: %d\n", stops);
    } else {
        printf("Impossible to reach the target.\n");
    }

    free(stations);
    free(heap);
    return 0;
}