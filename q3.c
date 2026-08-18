#include <stdio.h>
#include <stdlib.h>

// Comparison function for qsort()
int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

// Binary Search
int binarySearch(int arr[], int n, int key, int start)
{
    int low = start;
    int high = n - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (arr[mid] == key)
            return mid;

        if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return -1;
}

// Recursive function to select k-1 elements
int findKSum(int arr[], int n, int k, int T,
             int start, int depth, int sum)
{
    // If k-1 elements have been selected
    if (depth == k - 1)
    {
        int target = T - sum;

        // Search target only after the selected elements
        // to avoid reusing an element
        if (binarySearch(arr, n, target, start) != -1)
            return 1;

        return 0;
    }

    for (int i = start; i < n; i++)
    {
        if (findKSum(arr, n, k, T,
                     i + 1, depth + 1,
                     sum + arr[i]))
        {
            return 1;
        }
    }

    return 0;
}

int main()
{
    int n, k, T;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int S[n];

    printf("Enter the elements:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &S[i]);

    printf("Enter k: ");
    scanf("%d", &k);

    printf("Enter T: ");
    scanf("%d", &T);

    // Sort the set
    qsort(S, n, sizeof(int), compare);

    // Need at least k elements
    if (k > n || k <= 0)
    {
        printf("Invalid value of k.\n");
        return 0;
    }

    if (findKSum(S, n, k, T, 0, 0, 0))
        printf("\nYes, %d elements add up to %d.\n", k, T);
    else
        printf("\nNo, %d elements add up to %d.\n", k, T);

    return 0;
}