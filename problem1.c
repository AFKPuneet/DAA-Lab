#include <stdio.h>
#include <math.h>

int main()
{
    double n;

    printf("Enter value of n: ");
    scanf("%lf", &n);

    // Functions
    double f1  = n * log2(n);
    double f2  = 12 * sqrt(n);
    double f3  = 1 / n;
    double f4  = n;
    double f5  = log2(n);
    double f6  = 100 * n;
    double f7  = 2 + 6 * n;
    double f8  = 0.5 * n;
    double f9  = pow(2, n);
    double f10 = pow(n, 2);
    double f11 = 32 * n * log2(n);

    printf("\nValues of functions for n = %.0f:\n\n", n);

    printf("n log2(n)     = %.2lf\n", f1);
    printf("12 sqrt(n)    = %.2lf\n", f2);
    printf("1/n           = %.6lf\n", f3);
    printf("n             = %.2lf\n", f4);
    printf("log2(n)       = %.2lf\n", f5);
    printf("100n          = %.2lf\n", f6);
    printf("2 + 6n        = %.2lf\n", f7);
    printf("0.5n          = %.2lf\n", f8);
    printf("2^n           = %.2lf\n", f9);
    printf("n^2           = %.2lf\n", f10);
    printf("32n log2(n)   = %.2lf\n", f11);

    return 0;
} 