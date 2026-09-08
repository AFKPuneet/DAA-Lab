#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 20
#define EPS 1e-9

/* Print Matrix */
void printMatrix(double A[MAX][MAX], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%8.2lf ", A[i][j]);

        printf("\n");
    }
}

/* (i) Matrix Addition */
void matrixAddition(double A[MAX][MAX],
                    double B[MAX][MAX],
                    double C[MAX][MAX],
                    int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

/* (ii) Matrix Multiplication */
void matrixMultiplication(double A[MAX][MAX],
                          double B[MAX][MAX],
                          double C[MAX][MAX],
                          int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = 0;

            for (int k = 0; k < n; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/* (iii) Check Zero Matrix */
int isZeroMatrix(double A[MAX][MAX], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (fabs(A[i][j]) > EPS)
                return 0;
        }
    }

    return 1;
}

/* (iv) Check Symmetric Matrix */
int isSymmetric(double A[MAX][MAX], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (fabs(A[i][j] - A[j][i]) > EPS)
                return 0;
        }
    }

    return 1;
}

/* Get Minor Matrix */
void getMinor(double A[MAX][MAX],
              double minor[MAX][MAX],
              int n,
              int row,
              int col)
{
    int r = 0, c;

    for (int i = 0; i < n; i++)
    {
        if (i == row)
            continue;

        c = 0;

        for (int j = 0; j < n; j++)
        {
            if (j == col)
                continue;

            minor[r][c] = A[i][j];
            c++;
        }

        r++;
    }
}

/* (v) Determinant using recursive cofactor expansion */
double determinant(double A[MAX][MAX], int n)
{
    if (n == 1)
        return A[0][0];

    if (n == 2)
        return A[0][0] * A[1][1]
             - A[0][1] * A[1][0];

    double minor[MAX][MAX];
    double det = 0;

    for (int j = 0; j < n; j++)
    {
        getMinor(A, minor, n, 0, j);

        if (j % 2 == 0)
            det += A[0][j] * determinant(minor, n - 1);
        else
            det -= A[0][j] * determinant(minor, n - 1);
    }

    return det;
}

/* (vi) In-place Transpose */
void transposeInPlace(double A[MAX][MAX], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            double temp = A[i][j];
            A[i][j] = A[j][i];
            A[j][i] = temp;
        }
    }
}

/*
   (vii) Eigenvalue and Eigenvector
   Power Iteration method.

   Finds the dominant (largest magnitude)
   eigenvalue and corresponding eigenvector.
*/
void powerIteration(double A[MAX][MAX],
                    int n,
                    double *eigenvalue,
                    double eigenvector[MAX])
{
    double x[MAX];
    double y[MAX];

    /* Initial vector */
    for (int i = 0; i < n; i++)
        x[i] = 1.0;

    for (int iteration = 0; iteration < 1000; iteration++)
    {
        /* y = A*x */
        for (int i = 0; i < n; i++)
        {
            y[i] = 0;

            for (int j = 0; j < n; j++)
                y[i] += A[i][j] * x[j];
        }

        /* Find maximum absolute value */
        double maxValue = fabs(y[0]);

        for (int i = 1; i < n; i++)
        {
            if (fabs(y[i]) > maxValue)
                maxValue = fabs(y[i]);
        }

        if (maxValue < EPS)
            break;

        /* Normalize */
        for (int i = 0; i < n; i++)
            y[i] /= maxValue;

        /* Check convergence */
        double error = 0;

        for (int i = 0; i < n; i++)
            error += fabs(y[i] - x[i]);

        for (int i = 0; i < n; i++)
            x[i] = y[i];

        if (error < EPS)
            break;
    }

    /* Calculate eigenvalue using Rayleigh quotient */
    double Ax[MAX];

    for (int i = 0; i < n; i++)
    {
        Ax[i] = 0;

        for (int j = 0; j < n; j++)
            Ax[i] += A[i][j] * x[j];
    }

    double numerator = 0;
    double denominator = 0;

    for (int i = 0; i < n; i++)
    {
        numerator += x[i] * Ax[i];
        denominator += x[i] * x[i];
    }

    *eigenvalue = numerator / denominator;

    for (int i = 0; i < n; i++)
        eigenvector[i] = x[i];
}


int main()
{
    int n;

    double A[MAX][MAX];
    double B[MAX][MAX];
    double C[MAX][MAX];

    printf("Enter order of square matrix n: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX)
    {
        printf("Invalid matrix size.\n");
        return 1;
    }

    /* Input Matrix A */
    printf("\nEnter elements of Matrix A:\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            scanf("%lf", &A[i][j]);
    }

    /* Input Matrix B */
    printf("\nEnter elements of Matrix B:\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            scanf("%lf", &B[i][j]);
    }

    printf("\nMatrix A:\n");
    printMatrix(A, n);

    printf("\nMatrix B:\n");
    printMatrix(B, n);


    /* (i) Matrix Addition */
    matrixAddition(A, B, C, n);

    printf("\n(i) Matrix Addition (A + B):\n");
    printMatrix(C, n);


    /* (ii) Matrix Multiplication */
    matrixMultiplication(A, B, C, n);

    printf("\n(ii) Matrix Multiplication (A x B):\n");
    printMatrix(C, n);


    /* (iii) Zero Matrix */
    printf("\n(iii) Zero Matrix:\n");

    if (isZeroMatrix(A, n))
        printf("Matrix A is a zero matrix.\n");
    else
        printf("Matrix A is NOT a zero matrix.\n");


    /* (iv) Symmetric Matrix */
    printf("\n(iv) Symmetric Matrix:\n");

    if (isSymmetric(A, n))
        printf("Matrix A is symmetric.\n");
    else
        printf("Matrix A is NOT symmetric.\n");


    /* (v) Determinant */
    double det = determinant(A, n);

    printf("\n(v) Determinant of Matrix A = %.2lf\n", det);


    /* (vi) In-place Transpose */
    transposeInPlace(A, n);

    printf("\n(vi) Transpose of Matrix A (In-place):\n");
    printMatrix(A, n);


    /* (vii) Eigenvalue and Eigenvector */
    double eigenvalue;
    double eigenvector[MAX];

    powerIteration(B, n, &eigenvalue, eigenvector);

    printf("\n(vii) Dominant Eigenvalue of Matrix B = %.6lf\n",
           eigenvalue);

    printf("Corresponding Eigenvector:\n");

    for (int i = 0; i < n; i++)
        printf("%.6lf\n", eigenvector[i]);


    return 0;
}