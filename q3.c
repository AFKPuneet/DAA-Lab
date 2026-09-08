#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#define PI acos(-1.0)

/* Recursive FFT */
void FFT(complex double *a, int n, int invert)
{
    if (n == 1)
        return;

    complex double *even = malloc(n / 2 * sizeof(complex double));
    complex double *odd  = malloc(n / 2 * sizeof(complex double));

    for (int i = 0; i < n / 2; i++)
    {
        even[i] = a[2 * i];
        odd[i]  = a[2 * i + 1];
    }

    FFT(even, n / 2, invert);
    FFT(odd, n / 2, invert);

    double angle = 2 * PI / n * (invert ? -1 : 1);
    complex double w = 1;
    complex double wn = cos(angle) + I * sin(angle);

    for (int i = 0; i < n / 2; i++)
    {
        complex double t = w * odd[i];

        a[i] = even[i] + t;
        a[i + n / 2] = even[i] - t;

        w *= wn;
    }

    free(even);
    free(odd);
}

/* Convolution using FFT */
void convolution(double A[], int m,
                 double B[], int n,
                 double C[])
{
    int size = 1;

    while (size < m + n - 1)
        size *= 2;

    complex double *FA =
        calloc(size, sizeof(complex double));

    complex double *FB =
        calloc(size, sizeof(complex double));

    /* Copy input vectors */
    for (int i = 0; i < m; i++)
        FA[i] = A[i];

    for (int i = 0; i < n; i++)
        FB[i] = B[i];

    /* FFT */
    FFT(FA, size, 0);
    FFT(FB, size, 0);

    /* Point-wise multiplication */
    for (int i = 0; i < size; i++)
        FA[i] *= FB[i];

    /* Inverse FFT */
    FFT(FA, size, 1);

    /* Divide by size */
    for (int i = 0; i < m + n - 1; i++)
        C[i] = creal(FA[i]) / size;

    free(FA);
    free(FB);
}

int main()
{
    int m, n;

    printf("Enter size of vector A: ");
    scanf("%d", &m);

    printf("Enter size of vector B: ");
    scanf("%d", &n);

    double *A = malloc(m * sizeof(double));
    double *B = malloc(n * sizeof(double));
    double *C = malloc((m + n - 1) * sizeof(double));

    printf("Enter elements of A:\n");

    for (int i = 0; i < m; i++)
        scanf("%lf", &A[i]);

    printf("Enter elements of B:\n");

    for (int i = 0; i < n; i++)
        scanf("%lf", &B[i]);

    convolution(A, m, B, n, C);

    printf("\nConvolution C:\n");

    for (int i = 0; i < m + n - 1; i++)
        printf("%.2lf ", C[i]);

    printf("\n");

    free(A);
    free(B);
    free(C);

    return 0;
}