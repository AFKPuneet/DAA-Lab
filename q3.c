#include <stdio.h>
#include <stdlib.h>

#define INF 1000000000

int dp[100][100];
int split[100];

int min(int a, int b) {
    return a < b ? a : b;
}

int hanoi4(int n) {
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    if (dp[n][4] != -1)
        return dp[n][4];

    int best = INF;
    int bestK = 1;

    for (int k = 1; k < n; k++) {
        int moves = 2 * hanoi4(k) + ((1 << (n - k)) - 1);

        if (moves < best) {
            best = moves;
            bestK = k;
        }
    }

    split[n] = bestK;
    return dp[n][4] = best;
}

void move3(int n, int from, int to, int aux) {
    if (n == 0)
        return;

    move3(n - 1, from, aux, to);
    printf("%d %d\n", from, to);
    move3(n - 1, aux, to, from);
}

void move4(int n, int from, int to, int aux1, int aux2) {
    if (n == 0)
        return;

    if (n == 1) {
        printf("%d %d\n", from, to);
        return;
    }

    int k = split[n];

    move4(k, from, aux1, to, aux2);
    move3(n - k, from, to, aux2);
    move4(k, aux1, to, from, aux2);
}

int main() {
    int n;
    scanf("%d", &n);

    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
            dp[i][j] = -1;

    int moves = hanoi4(n);

    printf("%d\n", moves);

    move4(n, 1, 4, 2, 3);

    return 0;
}