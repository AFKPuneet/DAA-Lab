#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGS 100
#define MAX_INPUT_LENGTH 999

int overlap_length(const char *a, const char *b) {
    int len_a = (int)strlen(a);
    int len_b = (int)strlen(b);
    int limit = len_a < len_b ? len_a : len_b;

    for (int length = limit; length > 0; length--) {
        if (strncmp(a + len_a - length, b, (size_t)length) == 0) {
            return length;
        }
    }
    return 0;
}

char *merge_strings(const char *a, const char *b, int overlap) {
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    size_t result_len = len_a + len_b - (size_t)overlap;

    char *result = malloc(result_len + 1);
    if (result == NULL) return NULL;

    memcpy(result, a, len_a);
    memcpy(result + len_a, b + overlap, len_b - (size_t)overlap);
    result[result_len] = '\0';
    return result;
}

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_STRINGS) {
        fprintf(stderr, "Enter between 1 and %d strings.\n", MAX_STRINGS);
        return 1;
    }

    char *strings[MAX_STRINGS];

    for (int i = 0; i < n; i++) {
        char input[MAX_INPUT_LENGTH + 1];

        if (scanf("%999s", input) != 1) {
            fprintf(stderr, "Could not read string.\n");
            for (int j = 0; j < i; j++) free(strings[j]);
            return 1;
        }

        strings[i] = malloc(strlen(input) + 1);
        if (strings[i] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            for (int j = 0; j < i; j++) free(strings[j]);
            return 1;
        }
        strcpy(strings[i], input);
    }

    while (n > 1) {
        int best_i = 0, best_j = 1;
        int best_overlap = -1;
        int reverse_order = 0;

        /* Find the pair and direction with maximum suffix-prefix overlap. */
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int forward = overlap_length(strings[i], strings[j]);
                int backward = overlap_length(strings[j], strings[i]);

                if (forward > best_overlap) {
                    best_overlap = forward;
                    best_i = i;
                    best_j = j;
                    reverse_order = 0;
                }

                if (backward > best_overlap) {
                    best_overlap = backward;
                    best_i = i;
                    best_j = j;
                    reverse_order = 1;
                }
            }
        }

        const char *left = reverse_order ? strings[best_j] : strings[best_i];
        const char *right = reverse_order ? strings[best_i] : strings[best_j];

        char *merged = merge_strings(left, right, best_overlap);
        if (merged == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            for (int i = 0; i < n; i++) free(strings[i]);
            return 1;
        }

        free(strings[best_i]);
        free(strings[best_j]);
        strings[best_i] = merged;

        /* Remove the second selected string from the active list. */
        for (int i = best_j; i < n - 1; i++) {
            strings[i] = strings[i + 1];
        }
        n--;
    }

    printf("Greedy superstring: %s\n", strings[0]);

    free(strings[0]);
    return 0;
}