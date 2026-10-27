#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_WORDS 100001
#define MAX_LENGTH 10

static char words[MAX_WORDS][MAX_LENGTH + 1];

uint64_t hash_word(const char *s) {
    uint64_t hash = 1469598103934665603ULL;
    while (*s) {
        hash ^= (unsigned char)*s++;
        hash *= 1099511628211ULL;
    }
    return hash;
}

int lookup(const char *word, int table[], int table_size) {
    int slot = (int)(hash_word(word) & (uint64_t)(table_size - 1));

    while (table[slot] != -1) {
        int index = table[slot];
        if (strcmp(words[index], word) == 0) return index;
        slot = (slot + 1) & (table_size - 1);
    }

    return -1;
}

void insert_word(int index, int table[], int table_size) {
    int slot = (int)(hash_word(words[index]) & (uint64_t)(table_size - 1));

    while (table[slot] != -1) {
        slot = (slot + 1) & (table_size - 1);
    }
    table[slot] = index;
}

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n < 1 || n > 100000) {
        fprintf(stderr, "Dictionary size must be between 1 and 100000.\n");
        return 1;
    }

    int word_length = -1;

    for (int i = 0; i < n; i++) {
        if (scanf("%10s", words[i]) != 1) {
            fprintf(stderr, "Could not read dictionary word.\n");
            return 1;
        }

        int length = (int)strlen(words[i]);
        if (word_length == -1) word_length = length;
        if (length != word_length || length > MAX_LENGTH) {
            fprintf(stderr, "All words must have the same length (at most 10).\n");
            return 1;
        }
    }

    char start[MAX_LENGTH + 1], target[MAX_LENGTH + 1];
    if (scanf("%10s %10s", start, target) != 2 ||
        strlen(start) != (size_t)word_length ||
        strlen(target) != (size_t)word_length) {
        fprintf(stderr, "Invalid start or target word.\n");
        return 1;
    }

    if (strcmp(start, target) == 0) {
        printf("Minimum mutations: 0\n");
        return 0;
    }

    int table_size = 1;
    while (table_size < 2 * (n + 1)) table_size *= 2;

    int *table = malloc((size_t)table_size * sizeof(int));
    int *dist_start = malloc((size_t)(n + 1) * sizeof(int));
    int *dist_target = malloc((size_t)(n + 1) * sizeof(int));
    int *front_start = malloc((size_t)(n + 1) * sizeof(int));
    int *front_target = malloc((size_t)(n + 1) * sizeof(int));
    int *next_frontier = malloc((size_t)(n + 1) * sizeof(int));

    if (!table || !dist_start || !dist_target ||
        !front_start || !front_target || !next_frontier) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(table); free(dist_start); free(dist_target);
        free(front_start); free(front_target); free(next_frontier);
        return 1;
    }

    for (int i = 0; i < table_size; i++) table[i] = -1;
    for (int i = 0; i <= n; i++) {
        dist_start[i] = -1;
        dist_target[i] = -1;
    }
    for (int i = 0; i < n; i++) insert_word(i, table, table_size);

    int start_index = lookup(start, table, table_size);
    if (start_index == -1) {
        start_index = n;
        strcpy(words[n], start);
        insert_word(n, table, table_size);
        n++;
    }

    int target_index = lookup(target, table, table_size);
    if (target_index == -1) {
        printf("Minimum mutations: -1 (target is not in the dictionary)\n");
        free(table); free(dist_start); free(dist_target);
        free(front_start); free(front_target); free(next_frontier);
        return 0;
    }

    int count_start = 1, count_target = 1;
    front_start[0] = start_index;
    front_target[0] = target_index;
    dist_start[start_index] = 0;
    dist_target[target_index] = 0;

    int answer = -1;

    while (count_start > 0 && count_target > 0) {
        int expand_start = count_start <= count_target;
        int *frontier = expand_start ? front_start : front_target;
        int frontier_count = expand_start ? count_start : count_target;
        int *own_dist = expand_start ? dist_start : dist_target;
        int *other_dist = expand_start ? dist_target : dist_start;

        int next_count = 0;
        int best_meeting = -1;

        for (int i = 0; i < frontier_count; i++) {
            int u = frontier[i];
            char candidate[MAX_LENGTH + 1];
            strcpy(candidate, words[u]);

            for (int pos = 0; pos < word_length; pos++) {
                char original = candidate[pos];

                for (char ch = 'a'; ch <= 'z'; ch++) {
                    if (ch == original) continue;
                    candidate[pos] = ch;

                    int v = lookup(candidate, table, table_size);
                    if (v == -1) continue;

                    if (own_dist[v] == -1) {
                        own_dist[v] = own_dist[u] + 1;
                        next_frontier[next_count++] = v;
                    }

                    if (other_dist[v] != -1) {
                        int total = own_dist[v] + other_dist[v];
                        if (best_meeting == -1 || total < best_meeting) {
                            best_meeting = total;
                        }
                    }
                }

                candidate[pos] = original;
            }
        }

        if (best_meeting != -1) {
            answer = best_meeting;
            break;
        }

        if (expand_start) {
            memcpy(front_start, next_frontier,
                   (size_t)next_count * sizeof(int));
            count_start = next_count;
        } else {
            memcpy(front_target, next_frontier,
                   (size_t)next_count * sizeof(int));
            count_target = next_count;
        }
    }

    printf("Minimum mutations: %d\n", answer);

    free(table);
    free(dist_start);
    free(dist_target);
    free(front_start);
    free(front_target);
    free(next_frontier);
    return 0;
}