#include <stdio.h>
#include <string.h>

#define MAX_LEN 100000

typedef struct {
    char ch;
    int count;
} HeapNode;

typedef struct {
    char ch;
    int count;
    int release_position;
} CooldownNode;

void heap_push(HeapNode heap[], int *size, HeapNode node) {
    int i = (*size)++;
    heap[i] = node;

    while (i > 0) {
        int parent = (i - 1) / 2;

        if (heap[parent].count >= heap[i].count) break;

        HeapNode temp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = temp;
        i = parent;
    }
}

HeapNode heap_pop(HeapNode heap[], int *size) {
    HeapNode top = heap[0];
    heap[0] = heap[--(*size)];

    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < *size && heap[left].count > heap[largest].count)
            largest = left;
        if (right < *size && heap[right].count > heap[largest].count)
            largest = right;
        if (largest == i) break;

        HeapNode temp = heap[i];
        heap[i] = heap[largest];
        heap[largest] = temp;
        i = largest;
    }

    return top;
}

int main(void) {
    char input[MAX_LEN + 1];
    int k;

    if (scanf("%100000s %d", input, &k) != 2 || k < 0) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }

    int frequency[256] = {0};
    int length = (int)strlen(input);

    for (int i = 0; i < length; i++) {
        frequency[(unsigned char)input[i]]++;
    }

    if (k <= 1) {
        printf("Reorganised string: %s\n", input);
        return 0;
    }

    HeapNode heap[256];
    int heap_size = 0;

    for (int c = 0; c < 256; c++) {
        if (frequency[c] > 0) {
            HeapNode node = {(char)c, frequency[c]};
            heap_push(heap, &heap_size, node);
        }
    }

    CooldownNode cooldown[MAX_LEN];
    int front = 0, back = 0;
    char result[MAX_LEN + 1];
    int result_length = 0;

    for (int position = 0; position < length; position++) {
        /* Return characters whose cooldown has expired. */
        while (front < back &&
               cooldown[front].release_position <= position) {
            HeapNode node = {cooldown[front].ch, cooldown[front].count};
            heap_push(heap, &heap_size, node);
            front++;
        }

        if (heap_size == 0) {
            printf("Reorganised string: \n");
            return 0;
        }

        HeapNode chosen = heap_pop(heap, &heap_size);
        result[result_length++] = chosen.ch;
        chosen.count--;

        if (chosen.count > 0) {
            cooldown[back].ch = chosen.ch;
            cooldown[back].count = chosen.count;
            cooldown[back].release_position = position + k;
            back++;
        }
    }

    result[result_length] = '\0';
    printf("Reorganised string: %s\n", result);
    return 0;
}