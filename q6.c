#include <stdio.h>
#include <stdlib.h>

struct Event
{
    int point;
    int type;   // +1 = start, -1 = end
};

// Sort by point.
// If points are equal, start (+1) comes before end (-1).
int compare(const void *a, const void *b)
{
    struct Event *e1 = (struct Event *)a;
    struct Event *e2 = (struct Event *)b;

    if (e1->point != e2->point)
        return e1->point - e2->point;

    return e2->type - e1->type;
}

int main()
{
    int n;

    printf("Enter number of intervals: ");
    scanf("%d", &n);

    struct Event events[2 * n];

    printf("Enter the intervals (left right):\n");

    for (int i = 0; i < n; i++)
    {
        int l, r;

        scanf("%d %d", &l, &r);

        events[2 * i].point = l;
        events[2 * i].type = 1;

        events[2 * i + 1].point = r;
        events[2 * i + 1].type = -1;
    }

    // Sort all events
    qsort(events, 2 * n, sizeof(struct Event), compare);

    int current = 0;
    int maximum = 0;
    int maxPoint = 0;

    // Sweep from left to right
    for (int i = 0; i < 2 * n; i++)
    {
        current += events[i].type;

        if (current > maximum)
        {
            maximum = current;
            maxPoint = events[i].point;
        }
    }

    printf("\nPoint with maximum overlap = %d\n", maxPoint);
    printf("Maximum number of intervals = %d\n", maximum);

    return 0;
}