#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int year;
    int type;
} Event;

int compare(const void *a, const void *b) {
    Event *x = (Event *)a;
    Event *y = (Event *)b;

    if (x->year != y->year)
        return x->year - y->year;

    return x->type - y->type;
}

int main() {
    int n;
    scanf("%d", &n);

    Event events[2 * n];

    for (int i = 0; i < n; i++) {
        int birth, death;
        scanf("%d %d", &birth, &death);

        events[2 * i].year = birth;
        events[2 * i].type = 1;

        events[2 * i + 1].year = death;
        events[2 * i + 1].type = 0;
    }

    qsort(events, 2 * n, sizeof(Event), compare);

    int alive = 0;
    int maximum = 0;
    int bestYear = 0;

    for (int i = 0; i < 2 * n; i++) {
        if (events[i].type == 0) {
            alive--;
        } else {
            alive++;

            if (alive > maximum) {
                maximum = alive;
                bestYear = events[i].year;
            }
        }
    }

    printf("%d %d\n", bestYear, maximum);

    return 0;
}