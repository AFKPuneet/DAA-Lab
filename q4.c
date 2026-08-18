#include <stdio.h>

#define MAX 64

void add(int A[MAX][MAX], int B[MAX][MAX],
         int C[MAX][MAX], int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

void subtract(int A[MAX][MAX], int B[MAX][MAX],
              int C[MAX][MAX], int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

void strassen(int A[MAX][MAX], int B[MAX][MAX],
              int C[MAX][MAX], int n)
{
    if (n == 1)
    {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int k = n / 2;

    int A11[MAX][MAX], A12[MAX][MAX];
    int A21[MAX][MAX], A22[MAX][MAX];

    int B11[MAX][MAX], B12[MAX][MAX];
    int B21[MAX][MAX], B22[MAX][MAX];

    int M1[MAX][MAX], M2[MAX][MAX];
    int M3[MAX][MAX], M4[MAX][MAX];
    int M5[MAX][MAX], M6[MAX][MAX];
    int M7[MAX][MAX];

    int T1[MAX][MAX], T2[MAX][MAX];

    int C11[MAX][MAX], C12[MAX][MAX];
    int C21[MAX][MAX], C22[MAX][MAX];

    /* Divide matrices into four parts */

    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    /* M1 = (A11 + A22)(B11 + B22) */

    add(A11, A22, T1, k);
    add(B11, B22, T2, k);
    strassen(T1, T2, M1, k);

    /* M2 = (A21 + A22)B11 */

    add(A21, A22, T1, k);
    strassen(T1, B11, M2, k);

    /* M3 = A11(B12 - B22) */

    subtract(B12, B22, T2, k);
    strassen(A11, T2, M3, k);

    /* M4 = A22(B21 - B11) */

    subtract(B21, B11, T2, k);
    strassen(A22, T2, M4, k);

    /* M5 = (A11 + A12)B22 */

    add(A11, A12, T1, k);
    strassen(T1, B22, M5, k);

    /* M6 = (A21 - A11)(B11 + B12) */

    subtract(A21, A11, T1, k);
    add(B11, B12, T2, k);
    strassen(T1, T2, M6, k);

    /* M7 = (A12 - A22)(B21 + B22) */

    subtract(A12, A22, T1, k);
    add(B21, B22, T2, k);
    strassen(T1, T2, M7, k);

    /* Calculate C11 */

    add(M1, M4, T1, k);
    subtract(T1, M5, T2, k);
    add(T2, M7, C11, k);

    /* Calculate C12 */

    add(M3, M5, C12, k);

    /* Calculate C21 */

    add(M2, M4, C21, k);

    /* Calculate C22 */

    subtract(M1, M2, T1, k);
    add(T1, M3, T2, k);
    add(T2, M6, C22, k);

    /* Combine the four submatrices */

    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            C[i][j] = C11[i][j];
            C[i][j + k] = C12[i][j];
            C[i + k][j] = C21[i][j];
            C[i + k][j + k] = C22[i][j];
        }
    }
}

int main()
{
    int n;
    int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

    printf("Enter size of matrix: ");
    scanf("%d", &n);

    printf("Enter elements of Matrix A:\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            scanf("%d", &A[i][j]);
        }
    }

    printf("Enter elements of Matrix B:\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            scanf("%d", &B[i][j]);
        }
    }

    strassen(A, B, C, n);

    printf("\nResultant Matrix:\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", C[i][j]);
        }

        printf("\n");
    }

    return 0;
}