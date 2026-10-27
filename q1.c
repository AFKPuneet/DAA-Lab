#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#define EPS 1e-9

typedef struct {
    double value;
    double weight;
    double rate;       /* Initial value density: value / weight */
    double decay;      /* Density decreases by decay per time unit */
    double remaining;  /* Remaining available weight */
    double used;       /* Total weight selected */
} Item;

int main(void) {
    int n;
    double capacity;

    if (scanf("%d %lf", &n, &capacity) != 2 || n <= 0 || capacity < 0) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }

    Item *items = calloc((size_t)n, sizeof(Item));
    if (items == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (scanf("%lf %lf %lf",
                  &items[i].value, &items[i].weight, &items[i].decay) != 3 ||
            items[i].weight <= 0 || items[i].decay <= 0) {
            fprintf(stderr, "Each item needs positive weight and decay rate.\n");
            free(items);
            return 1;
        }

        items[i].rate = items[i].value / items[i].weight;
        items[i].remaining = items[i].weight;
    }

    double time = 0.0;
    double total_value = 0.0;
    double capacity_left = capacity;

    printf("Processing schedule (item, weight processed, start time, end time):\n");

    while (capacity_left > EPS) {
        int chosen = -1;
        double best_density = -DBL_MAX;

        /* Find the available item with the highest current density. */
        for (int i = 0; i < n; i++) {
            if (items[i].remaining <= EPS) continue;

            double density = items[i].rate - items[i].decay * time;

            if (density > best_density + EPS ||
                (fabs(density - best_density) <= EPS &&
                 chosen >= 0 && items[i].decay < items[chosen].decay)) {
                best_density = density;
                chosen = i;
            }
        }

        /* No positive-value work remains. */
        if (chosen < 0 || best_density <= EPS) {
            break;
        }

        double duration = capacity_left;

        /* Stop when this item's available weight is exhausted. */
        if (items[chosen].remaining < duration) {
            duration = items[chosen].remaining;
        }

        /* Stop when its density reaches zero. */
        double zero_time = items[chosen].rate / items[chosen].decay;
        double until_zero = zero_time - time;
        if (until_zero < duration) {
            duration = until_zero;
        }

        /*
         * Stop when another remaining item's density catches up.
         * Only an item with a slower decay rate can catch the chosen item.
         */
        for (int j = 0; j < n; j++) {
            if (j == chosen || items[j].remaining <= EPS) continue;

            double decay_difference = items[chosen].decay - items[j].decay;
            if (decay_difference <= EPS) continue;

            double crossing_time =
                (items[chosen].rate - items[j].rate) / decay_difference;

            double until_crossing = crossing_time - time;
            if (until_crossing > EPS && until_crossing < duration) {
                duration = until_crossing;
            }
        }

        if (duration <= EPS) {
            break;
        }

        double start_time = time;
        double end_time = time + duration;

        /* Integrate density over this processing interval. */
        total_value += items[chosen].rate * duration
                     - 0.5 * items[chosen].decay
                       * (end_time * end_time - start_time * start_time);

        items[chosen].remaining -= duration;
        items[chosen].used += duration;
        capacity_left -= duration;
        time = end_time;

        printf("%d, %.6f, %.6f, %.6f\n",
               chosen + 1, duration, start_time, end_time);
    }

    printf("Total value: %.6f\n", total_value);
    printf("Selected weight by item:\n");

    for (int i = 0; i < n; i++) {
        printf("Item %d: %.6f\n", i + 1, items[i].used);
    }

    printf("Total weight selected: %.6f\n", capacity - capacity_left);

    free(items);
    return 0;
}