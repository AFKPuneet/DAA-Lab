#include <stdio.h>

long long solve(int n) {
    if (n == 0)
        return 0;

    if (n == 1)
        return 1;

    return ((1LL << (n + 1)) - 1) / 3;
}

int main() {
    int n;
    scanf("%d", &n);

    printf("%lld\n", solve(n));

    return 0;
}