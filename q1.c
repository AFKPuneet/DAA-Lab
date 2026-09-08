#include <stdio.h>
#include <math.h>

int main() {
    int n;
    printf("Enter the no:");
    scanf("%d", &n);

    if (n <= 1)
        printf("0\n");
    else if (n == 2)
        printf("1\n");
    else
        printf("%d\n", (n * n - 5 * n + 10) / 2);

    return 0;

}