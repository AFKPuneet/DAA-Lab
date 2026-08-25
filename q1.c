#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }

    swap(&arr[i], &arr[high]);
    return i;
}

int quickSelect(int arr[], int low, int high, int k) {
    if (low == high)
        return arr[low];

    int pivotIndex = partition(arr, low, high);

    if (pivotIndex == k)
        return arr[pivotIndex];
    else if (pivotIndex > k)
        return quickSelect(arr, low, pivotIndex - 1, k);
    else
        return quickSelect(arr, pivotIndex + 1, high, k);
}

int main() {
    int n;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];

    printf("Enter %d numbers:\n", n);

    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    if (n % 2 != 0) {
        // Odd number of elements
        int median = quickSelect(arr, 0, n - 1, n / 2);
        printf("Median = %d\n", median);
    } 
    else {
        // Even number of elements
        int a = quickSelect(arr, 0, n - 1, n / 2 - 1);
        int b = quickSelect(arr, 0, n - 1, n / 2);

        float median = (a + b) / 2.0;
        printf("Median = %.2f\n", median);
    }

    return 0;
}