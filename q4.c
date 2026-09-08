#include <stdio.h>
#include <stdlib.h>

/* Global counter for total reversal cost */
long long reversalCost = 0;

/*
   Reverse p[i...j]

   Cost = |j - i| + 1
*/
void reverse(int p[], int i, int j)
{
    if (i >= j)
    {
        if (i == j)
            reversalCost += 1;

        return;
    }

    reversalCost += (j - i + 1);

    while (i < j)
    {
        int temp = p[i];
        p[i] = p[j];
        p[j] = temp;

        i++;
        j--;
    }
}

/*
   Lower bound:
   Find first position in [left, right]
   whose value is >= key.
*/
int lowerBound(int p[], int left, int right, int key)
{
    int low = left;
    int high = right + 1;

    while (low < high)
    {
        int mid = low + (high - low) / 2;

        if (p[mid] < key)
            low = mid + 1;
        else
            high = mid;
    }

    return low;
}

/*
   Rotate two adjacent portions:

   [first ... middle] [middle+1 ... last]

   into

   [middle+1 ... last] [first ... middle]

   using three reversals.
*/
void rotate(int p[], int first, int middle, int last)
{
    if (first > middle || middle >= last)
        return;

    reverse(p, first, middle);
    reverse(p, middle + 1, last);
    reverse(p, first, last);
}

/*
   Merge two sorted ranges:

   [left ... middle] and [middle+1 ... right]

   using only reversals.
*/
void mergeByReversal(int p[], int left, int middle, int right)
{
    if (left >= middle || middle >= right)
        return;

    /*
       If already sorted, nothing is needed.
    */
    if (p[middle] <= p[middle + 1])
        return;

    /*
       Divide the larger half.
    */
    int firstCut;
    int secondCut;

    if (middle - left > right - middle)
    {
        /* Divide left half */
        firstCut = left + (middle - left) / 2;

        secondCut =
            lowerBound(p,
                       middle + 1,
                       right,
                       p[firstCut]);
    }
    else
    {
        /* Divide right half */
        secondCut =
            middle + 1 +
            (right - middle) / 2;

        firstCut =
            lowerBound(p,
                       left,
                       middle,
                       p[secondCut]);
    }

    /*
       If cuts do not create useful sections,
       return.
    */
    if (firstCut > middle ||
        secondCut > right + 1)
        return;

    /*
       Rotate:
       
       [firstCut ... middle]
       [middle+1 ... secondCut-1]

       into

       [middle+1 ... secondCut-1]
       [firstCut ... middle]
    */
    int newMiddle =
        firstCut + (secondCut - (middle + 1)) - 1;

    rotate(p,
           firstCut,
           middle,
           secondCut - 1);

    /*
       Recursively merge first part.
    */
    mergeByReversal(p,
                    left,
                    firstCut - 1,
                    newMiddle);

    /*
       Recursively merge second part.
    */
    mergeByReversal(p,
                    newMiddle + 1,
                    secondCut - 1,
                    right);
}

/*
   Merge Sort using reversal-based merging.
*/
void reversalMergeSort(int p[], int left, int right)
{
    if (left >= right)
        return;

    int middle = left + (right - left) / 2;

    /* Sort left half */
    reversalMergeSort(p, left, middle);

    /* Sort right half */
    reversalMergeSort(p, middle + 1, right);

    /* Merge using reversals */
    mergeByReversal(p, left, middle, right);
}

/* Print array */
void printArray(int p[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", p[i]);

    printf("\n");
}

int main()
{
    int n;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int *p = malloc(n * sizeof(int));

    printf("Enter permutation:\n");

    for (int i = 0; i < n; i++)
        scanf("%d", &p[i]);

    printf("\nOriginal permutation:\n");
    printArray(p, n);

    /* Sort */
    reversalMergeSort(p, 0, n - 1);

    printf("\nSorted permutation:\n");
    printArray(p, n);

    printf("\nTotal reversal cost = %lld\n",
           reversalCost);

    free(p);

    return 0;
}