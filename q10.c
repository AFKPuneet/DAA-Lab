#include <stdio.h>
#include <limits.h>

typedef struct {
    unsigned long long steps;
    unsigned long long maximum;
    int reached_one;
    int overflow;
} Result;

Result analyze_collatz(unsigned long long start,
                       unsigned long long step_limit) {
    Result result = {0, start, 0, 0};
    unsigned long long value = start;

    while (value != 1 && result.steps < step_limit) {
        if (value > result.maximum) {
            result.maximum = value;
        }

        if (value % 2 == 0) {
            value /= 2;
        } else {
            /* Check whether 3 * value + 1 would overflow. */
            if (value > (ULLONG_MAX - 1) / 3) {
                result.overflow = 1;
                return result;
            }
            value = 3 * value + 1;
        }

        result.steps++;
    }

    if (value > result.maximum) {
        result.maximum = value;
    }

    result.reached_one = (value == 1);
    return result;
}

int main(void) {
    unsigned long long a, b, step_limit;

    printf("Enter interval [a b] and maximum steps per start: ");
    if (scanf("%llu %llu %llu", &a, &b, &step_limit) != 3 ||
        a == 0 || b < a || step_limit == 0) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }

    for (unsigned long long start = a; ; start++) {
        Result r = analyze_collatz(start, step_limit);

        printf("%llu: steps=%llu, maximum=%llu, ",
               start, r.steps, r.maximum);

        if (r.overflow) {
            printf("overflow detected\n");
        } else if (r.reached_one) {
            printf("reached 1\n");
        } else {
            printf("step limit reached before 1\n");
        }

        if (start == b) {
            break;  /* Avoid overflow when b is ULLONG_MAX. */
        }
    }

    return 0;
}