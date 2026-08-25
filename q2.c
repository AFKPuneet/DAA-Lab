#include <stdio.h>

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

    else if (k < pivotIndex)
        return quickSelect(arr, low, pivotIndex - 1, k);

    else
        return quickSelect(arr, pivotIndex + 1, high, k);
}

int main() {
    int n, k;

    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int arr[n];

    printf("Enter the elements:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Enter K: ");
    scanf("%d", &k);

    if (k < 1 || k > n) {
        printf("Invalid value of K\n");
        return 0;
    }

    // Array index starts from 0, so K-1
    int result = quickSelect(arr, 0, n - 1, k - 1);

    printf("%dth smallest element is: %d\n", k, result);

    return 0;
}
