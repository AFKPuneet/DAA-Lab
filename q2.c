#include <stdio.h>

/*
    balance(a, b, n)

    Returns:
       -1 : left group is lighter
        0 : both groups have equal weight
        1 : right group is lighter

    For simulation:
    weights[] contains the actual weights of coins.
*/
int balance(int weights[], int leftStart, int rightStart, int n)
{
    int leftWeight = 0;
    int rightWeight = 0;

    for (int i = 0; i < n; i++)
    {
        leftWeight += weights[leftStart + i];
        rightWeight += weights[rightStart + i];
    }

    if (leftWeight < rightWeight)
        return -1;

    if (leftWeight > rightWeight)
        return 1;

    return 0;
}


/*
    Recursive divide-and-conquer function.

    candidateStart : starting index of possible defective coins
    candidateSize  : number of possible defective coins
    goodCoin       : index of a coin known to be good

    Returns:
       defective coin index
       -1 if no defective coin exists
*/
int findDefective(int weights[], int candidateStart,
                  int candidateSize, int goodCoin)
{
    // One possible coin remains
    if (candidateSize == 1)
    {
        if (weights[candidateStart] < weights[goodCoin])
            return candidateStart;

        return -1;
    }

    // Divide candidate coins into two groups
    int leftSize = candidateSize / 2;
    int rightSize = candidateSize / 2;

    int remaining = candidateSize - leftSize - rightSize;

    int leftStart = candidateStart;
    int rightStart = candidateStart + leftSize;

    // Compare equal-sized groups
    int result = balance(weights, leftStart, rightStart, leftSize);

    // Left group is lighter
    if (result == -1)
    {
        return findDefective(weights,
                             leftStart,
                             leftSize,
                             rightStart);
    }

    // Right group is lighter
    if (result == 1)
    {
        return findDefective(weights,
                             rightStart,
                             rightSize,
                             leftStart);
    }

    /*
        Both groups are equal.

        Therefore, all coins in left and right
        groups are definitely good.

        Any remaining coin(s), or NONE, can
        contain the defective coin.
    */
    if (remaining > 0)
    {
        int remainingStart = rightStart + rightSize;

        return findDefective(weights,
                             remainingStart,
                             remaining,
                             leftStart);
    }

    // No coins remain and both groups were equal
    return -1;
}


int main()
{
    int n;

    printf("Enter number of coins: ");
    scanf("%d", &n);

    int weights[n];

    printf("Enter weights of %d coins:\n", n);

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &weights[i]);
    }

    /*
        First, find a known-good coin.

        Since at most one coin is defective,
        if the first two coins have equal weight,
        either one can be used as a good coin.

        For a general implementation, we use
        the divide-and-conquer search directly.
    */

    if (n == 1)
    {
        printf("\nNo defective coin can be identified with only one coin.\n");
        return 0;
    }

    /*
        We use coin 0 as a reference.

        To correctly handle the possibility that
        coin 0 itself is defective, first compare
        groups excluding the reference when necessary.

        The following implementation uses a simpler
        recursive search based on the complete array.
    */

    int defective = -1;

    /*
        Try every possible coin as the defective candidate
        through a binary divide-and-conquer strategy.
    */

    // Determine a known-good coin.
    int goodCoin = -1;

    for (int i = 1; i < n; i++)
    {
        if (weights[i] == weights[0])
        {
            goodCoin = i;
            break;
        }
    }

    if (goodCoin == -1)
    {
        /*
            This case can occur in the simulation when
            n is very small or when coin 0 is defective.
            Find two equal coins.
        */
        for (int i = 0; i < n && goodCoin == -1; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (weights[i] == weights[j])
                {
                    goodCoin = i;
                    break;
                }
            }
        }
    }

    if (goodCoin != -1)
    {
        /*
            Search all coins except the known-good coin.
        */
        int candidate[n - 1];
        int k = 0;

        for (int i = 0; i < n; i++)
        {
            if (i != goodCoin)
                candidate[k++] = i;
        }

        /*
            Binary divide-and-conquer on candidates.
        */
        int low = 0;
        int high = k - 1;

        while (low <= high)
        {
            int mid = (low + high) / 2;

            if (weights[candidate[mid]] < weights[goodCoin])
            {
                defective = candidate[mid];
                break;
            }

            if (weights[candidate[mid]] == weights[goodCoin])
            {
                /*
                    This particular coin is good,
                    so continue searching.
                */
                low = mid + 1;
            }
            else
            {
                high = mid - 1;
            }
        }
    }

    if (defective == -1)
        printf("\nNo defective coin found.\n");
    else
        printf("\nDefective coin = Coin %d\n", defective + 1);

    return 0;
}