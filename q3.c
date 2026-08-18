#include <stdio.h>

struct MinMax
{
    int min;
    int max;
};

struct MinMax findMinMax(int arr[], int low, int high)
{
    struct MinMax result;
    struct MinMax left;
    struct MinMax right;

    // Case 1: Only one element
    if (low == high)
    {
        result.min = arr[low];
        result.max = arr[low];

        return result;
    }

    // Case 2: Two elements
    if (high == low + 1)
    {
        if (arr[low] < arr[high])
        {
            result.min = arr[low];
            result.max = arr[high];
        }
        else
        {
            result.min = arr[high];
            result.max = arr[low];
        }

        return result;
    }

    // Divide
    int mid = (low + high) / 2;

    // Conquer
    left = findMinMax(arr, low, mid);
    right = findMinMax(arr, mid + 1, high);

    // Combine
    if (left.max > right.max)
        result.max = left.max;
    else
        result.max = right.max;

    if (left.min < right.min)
        result.min = left.min;
    else
        result.min = right.min;

    return result;
}

int main()
{
    int n;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];

    printf("Enter %d elements:\n", n);

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    struct MinMax result = findMinMax(arr, 0, n - 1);

    printf("\nMinimum element = %d\n", result.min);
    printf("Maximum element = %d\n", result.max);

    return 0;
}