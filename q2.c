#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char symbol;
    unsigned long long frequency;
    int left;
    int right;
    int parent;
} Node;

typedef struct {
    char symbol;
    int length;
} SymbolCode;

void find_lengths(Node nodes[], int index, int depth,
                  SymbolCode codes[], int *count) {
    if (nodes[index].left == -1 && nodes[index].right == -1) {
        codes[*count].symbol = nodes[index].symbol;
        codes[*count].length = (depth == 0) ? 1 : depth;
        (*count)++;
        return;
    }

    find_lengths(nodes, nodes[index].left, depth + 1, codes, count);
    find_lengths(nodes, nodes[index].right, depth + 1, codes, count);
}

int compare_codes(const void *a, const void *b) {
    const SymbolCode *x = a;
    const SymbolCode *y = b;

    if (x->length != y->length) return x->length - y->length;
    return (unsigned char)x->symbol - (unsigned char)y->symbol;
}

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid number of symbols.\n");
        return 1;
    }

    int total_nodes = 2 * n - 1;
    Node *nodes = calloc((size_t)total_nodes, sizeof(Node));
    SymbolCode *codes = malloc((size_t)n * sizeof(SymbolCode));

    if (nodes == NULL || codes == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(nodes);
        free(codes);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        char symbol;
        unsigned long long frequency;

        if (scanf(" %c %llu", &symbol, &frequency) != 2 || frequency == 0) {
            fprintf(stderr, "Invalid symbol or frequency.\n");
            free(nodes);
            free(codes);
            return 1;
        }

        nodes[i].symbol = symbol;
        nodes[i].frequency = frequency;
        nodes[i].left = -1;
        nodes[i].right = -1;
        nodes[i].parent = -1;
    }

    /* Repeatedly combine the two lowest-frequency unparented nodes. */
    for (int next = n; next < total_nodes; next++) {
        int first = -1, second = -1;

        for (int i = 0; i < next; i++) {
            if (nodes[i].parent != -1) continue;

            if (first == -1 ||
                nodes[i].frequency < nodes[first].frequency) {
                second = first;
                first = i;
            } else if (second == -1 ||
                       nodes[i].frequency < nodes[second].frequency) {
                second = i;
            }
        }

        if (first == -1 || second == -1) {
            fprintf(stderr, "Could not construct Huffman tree.\n");
            free(nodes);
            free(codes);
            return 1;
        }

        nodes[next].frequency =
            nodes[first].frequency + nodes[second].frequency;
        nodes[next].left = first;
        nodes[next].right = second;
        nodes[next].parent = -1;

        nodes[first].parent = next;
        nodes[second].parent = next;
    }

    int count = 0;
    find_lengths(nodes, total_nodes - 1, 0, codes, &count);
    qsort(codes, (size_t)n, sizeof(SymbolCode), compare_codes);

    int max_length = codes[n - 1].length;
    char *binary = calloc((size_t)max_length + 2, sizeof(char));
    if (binary == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(nodes);
        free(codes);
        return 1;
    }

    /* First canonical code consists of all zero bits. */
    int current_length = codes[0].length;
    for (int i = 0; i < current_length; i++) binary[i] = '0';
    binary[current_length] = '\0';

    printf("Canonical Huffman codebook:\n");
    printf("%c %s\n", codes[0].symbol, binary);

    for (int i = 1; i < n; i++) {
        /* Increment the previous code as a binary number. */
        int position = current_length - 1;
        while (position >= 0 && binary[position] == '1') {
            binary[position] = '0';
            position--;
        }

        if (position < 0) {
            fprintf(stderr, "Code length overflow.\n");
            free(binary);
            free(nodes);
            free(codes);
            return 1;
        }

        binary[position] = '1';

        int new_length = codes[i].length;
        for (int k = current_length; k < new_length; k++) {
            binary[k] = '0';
        }
        binary[new_length] = '\0';
        current_length = new_length;

        printf("%c %s\n", codes[i].symbol, binary);
    }

    free(binary);
    free(nodes);
    free(codes);
    return 0;
}