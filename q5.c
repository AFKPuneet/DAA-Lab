#include <stdio.h>
#include <stdlib.h>

struct Interval
{
    int start;
    int end;
};

// Compare intervals based on starting point
int compare(const void *a, const void *b)
{
    struct Interval *i1 = (struct Interval *)a;
    struct Interval *i2 = (struct Interval *)b;

    return i1->start - i2->start;
}

int main()
{
    int n;

    printf("Enter number of intervals: ");
    scanf("%d", &n);

    struct Interval intervals[n];
    struct Interval result[n];

    printf("Enter the intervals (start end):\n");

    for (int i = 0; i < n; i++)
    {
        scanf("%d %d",
              &intervals[i].start,
              &intervals[i].end);
    }

    // Sort intervals according to start time
    qsort(intervals, n, sizeof(struct Interval), compare);

    int count = 0;

    // Start with the first interval
    int currentStart = intervals[0].start;
    int currentEnd = intervals[0].end;

    for (int i = 1; i < n; i++)
    {
        // Check if intervals overlap
        if (intervals[i].start <= currentEnd)
        {
            // Merge intervals
            if (intervals[i].end > currentEnd)
                currentEnd = intervals[i].end;
        }
        else
        {
            // Store the current merged interval
            result[count].start = currentStart;
            result[count].end = currentEnd;
            count++;

            // Start a new interval
            currentStart = intervals[i].start;
            currentEnd = intervals[i].end;
        }
    }

    // Store the last interval
    result[count].start = currentStart;
    result[count].end = currentEnd;
    count++;

    // Print merged intervals
    printf("\nMerged intervals:\n");

    for (int i = 0; i < count; i++)
    {
        printf("(%d,%d) ", result[i].start, result[i].end);
    }

    printf("\n");

    return 0;
}