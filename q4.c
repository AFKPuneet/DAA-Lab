#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    
    if (largest != i) {
        swap(&arr[i], &arr[largest]);

        
        heapify(arr, n, largest);
    }
}


void heapSort(int arr[], int n) {

   
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    
    for (int i = n - 1; i > 0; i--) {

       
        swap(&arr[0], &arr[i]);

       
        heapify(arr, i, 0);
    }
}

int main() {
    int n;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];

    FILE *fp;

   
    fp = fopen("random.txt", "w");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        int num = rand() % 1000;
        fprintf(fp, "%d ", num);
    }

    fclose(fp);

   
    fp = fopen("random.txt", "r");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(fp, "%d", &arr[i]);
    }

    fclose(fp);

   
    heapSort(arr, n);

    
    fp = fopen("sorted.txt", "w");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d ", arr[i]);
    }

    fclose(fp);

    printf("Random elements stored in random.txt\n");
    printf("Sorted elements stored in sorted.txt\n");

    return 0;
}