#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int cmp(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void printArray(int a[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);

    printf("\n");
}

void firstSecondLargest(int a[], int n)
{
    int first = a[0];
    int second = a[0];

    for (int i = 1; i < n; i++)
    {
        if (a[i] > first)
        {
            second = first;
            first = a[i];
        }
        else if (a[i] > second && a[i] != first)
        {
            second = a[i];
        }
    }

    printf("Largest = %d\n", first);
    printf("Second Largest = %d\n", second);
}

void median(int a[], int n)
{
    int *b = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        b[i] = a[i];

    qsort(b, n, sizeof(int), cmp);

    if (n % 2)
        printf("Median = %d\n", b[n / 2]);
    else
        printf("Median = %.2f\n",
               (b[n / 2 - 1] + b[n / 2]) / 2.0);

    free(b);
}

void mode(int a[], int n)
{
    int best = a[0];
    int bestCount = 1;

    for (int i = 0; i < n; i++)
    {
        int count = 0;

        for (int j = 0; j < n; j++)
        {
            if (a[j] == a[i])
                count++;
        }

        if (count > bestCount)
        {
            bestCount = count;
            best = a[i];
        }
    }

    printf("Mode = %d\n", best);
}

void removeDuplicates(int a[], int *n)
{
    for (int i = 0; i < *n; i++)
    {
        for (int j = i + 1; j < *n;)
        {
            if (a[i] == a[j])
            {
                for (int k = j; k < *n - 1; k++)
                    a[k] = a[k + 1];

                (*n)--;
            }
            else
            {
                j++;
            }
        }
    }
}

void reverse(int a[], int n)
{
    for (int i = 0; i < n / 2; i++)
    {
        int temp = a[i];
        a[i] = a[n - 1 - i];
        a[n - 1 - i] = temp;
    }
}

void partitionPivot(int a[], int n, int pivot)
{
    int *b = malloc(n * sizeof(int));
    int k = 0;

    for (int i = 0; i < n; i++)
    {
        if (a[i] < pivot)
            b[k++] = a[i];
    }

    for (int i = 0; i < n; i++)
    {
        if (a[i] >= pivot)
            b[k++] = a[i];
    }

    for (int i = 0; i < n; i++)
        a[i] = b[i];

    free(b);
}

int main()
{
    int n;

    printf("Enter n: ");
    scanf("%d", &n);

    int *a = malloc(n * sizeof(int));

    double sum = 0;
    double sd = 0;

    printf("Enter %d integers:\n", n);

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
        sum += a[i];
    }

    /* Maximum */
    int max = a[0];

    for (int i = 1; i < n; i++)
    {
        if (a[i] > max)
            max = a[i];
    }

    printf("Maximum = %d\n", max);

    /* Largest and second largest */
    firstSecondLargest(a, n);

    /* Mean */
    printf("Mean = %.2f\n", sum / n);

    /* Median */
    median(a, n);

    /* Standard deviation */
    for (int i = 0; i < n; i++)
        sd += pow(a[i] - sum / n, 2);

    printf("Standard Deviation = %.2f\n",
           sqrt(sd / n));

    /* Mode */
    mode(a, n);

    /* Remove duplicates */
    removeDuplicates(a, &n);

    printf("After removing duplicates: ");
    printArray(a, n);

    /* Reverse */
    reverse(a, n);

    printf("After reversing: ");
    printArray(a, n);

    /* Partition */
    int pivot;

    printf("Enter pivot: ");
    scanf("%d", &pivot);

    partitionPivot(a, n, pivot);

    printf("After partitioning: ");
    printArray(a, n);

    free(a);

    return 0;
}