#include <stdio.h>

// Binary Search
int binarySearch(int arr[], int n, int x, int *comparisons)
{
    int low = 0;
    int high = n - 1;

    *comparisons = 0;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        (*comparisons)++;

        if (arr[mid] == x)
            return mid;

        (*comparisons)++;

        if (x < arr[mid])
            high = mid - 1;
        else
            low = mid + 1;
    }

    return -1;
}


// Ternary Search
int ternarySearch(int arr[], int n, int x, int *comparisons)
{
    int low = 0;
    int high = n - 1;

    *comparisons = 0;

    while (low <= high)
    {
        int third = (high - low) / 3;

        int mid1 = low + third;
        int mid2 = high - third;

        (*comparisons)++;

        if (arr[mid1] == x)
            return mid1;

        (*comparisons)++;

        if (arr[mid2] == x)
            return mid2;

        (*comparisons)++;

        if (x < arr[mid1])
        {
            high = mid1 - 1;
        }
        else if (x > arr[mid2])
        {
            low = mid2 + 1;
        }
        else
        {
            low = mid1 + 1;
            high = mid2 - 1;
        }
    }

    return -1;
}


int main()
{
    int n, x;
    int binaryComparisons, ternaryComparisons;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];

    printf("Enter %d sorted elements:\n", n);

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    printf("\nEnter element to search: ");
    scanf("%d", &x);

    // Binary Search
    int binaryResult = binarySearch(
        arr, n, x, &binaryComparisons
    );

    // Ternary Search
    int ternaryResult = ternarySearch(
        arr, n, x, &ternaryComparisons
    );

    // Binary Search Result
    printf("\n--- Binary Search ---\n");

    if (binaryResult != -1)
        printf("Element found at position %d\n",
               binaryResult + 1);
    else
        printf("Element not found\n");

    printf("Number of comparisons: %d\n",
           binaryComparisons);


    // Ternary Search Result
    printf("\n--- Ternary Search ---\n");

    if (ternaryResult != -1)
        printf("Element found at position %d\n",
               ternaryResult + 1);
    else
        printf("Element not found\n");

    printf("Number of comparisons: %d\n",
           ternaryComparisons);


    // Comparison
    printf("\n--- Comparison ---\n");

    if (binaryComparisons < ternaryComparisons)
        printf("Binary Search is better.\n");
    else if (ternaryComparisons < binaryComparisons)
        printf("Ternary Search is better for this input.\n");
    else
        printf("Both performed the same number of comparisons.\n");

    return 0;
}