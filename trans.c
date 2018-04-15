/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int m, n, i, j;
    int temp[8]; //store numbers at temp[0]

    if (M == 32 && N == 32)
    {
        for (m = 0; m < 4; m++)
        {
            for (n = 0; n < 4; n++)
            {
                for (i = 0; i < 8; i++)
                {
                    for (j = 0; j < 8; j++)
                    {
                        if (i == j)
                            temp[0] = A[m * 8 + i][n * 8 + j];
                        else
                            B[n * 8 + j][m * 8 + i] = A[m * 8 + i][n * 8 + j];
                    }
                    B[n * 8 + i][m * 8 + i] = temp[0];
                }
            }
        }
    }
    else if (M == 64 && N == 64)
    {
        for (m = 0; m < 8; m++)
        {
            for (n = 0; n < 8; n++) //cross stroe
            {
                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 8; j++)
                        temp[j] = A[m * 8 + i][n * 8 + j];
                    for (j = 0; j < 4; j++)
                        B[n * 8 + j][m * 8 + i] = temp[j];
                    for (j = 0; j < 4; j++)
                        B[n * 8 + j][m * 8 + i + 4] = temp[j + 4];
                }
                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 4; j++)
                        temp[j + 4] = A[m * 8 + j + 4][n * 8 + i];
                    for (j = 0; j < 4; j++)
                        temp[j] = B[n * 8 + i][m * 8 + j + 4];
                    for (j = 0; j < 4; j++)
                        B[n * 8 + i][m * 8 + j + 4] = temp[j + 4];
                    for (j = 0; j < 4; j++)
                        B[n * 8 + i + 4][m * 8 + j] = temp[j];
                    for (j = 0; j < 4; j++)
                    {
                        if (i == j)
                            continue;
                        else
                            B[n * 8 + i + 4][m * 8 + j + 4] = A[m * 8 + j + 4][n * 8 + i + 4];
                    }
                }
                for (i = 0; i < 4; i++)
                    B[n * 8 + i + 4][m * 8 + i + 4] = A[m * 8 + i + 4][n * 8 + i + 4];
            }
        }
    }
    else
    {
        for (m = 0; m < N; m += 16)
        {
            for (n = 0; n < M; n += 16)
            {
                for (i = m; i < m + 16 && i < N; i++)
                {
                    for (j = n; j < n + 16 && j < M; j++)
                    {
                        B[j][i] = A[i][j];
                    }
                }
            }
        }
    }
}
/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/* 
 * trans_test
 */
char trans_desc_test[] = "trans_test, don't consider temp[0]";
void trans_test(int M, int N, int A[N][M], int B[M][N])
{
    int m, n, i, j;
    int temp[8];
    if (M == 32 && N == 32)
    {
        for (m = 0; m < 4; m++)
        {
            for (n = 0; n < 4; n++)
            {
                for (i = 0; i < 8; i++)
                {
                    for (j = 0; j < 8; j++)
                    {
                        B[n * 8 + j][m * 8 + i] = A[m * 8 + i][n * 8 + j];
                    }
                }
            }
        }
    }
    else if (M == 64 && N == 64)
    {
        for (m = 0; m < 8; m++)
        {
            for (n = 0; n < 8; n++) //cross stroe
            {
                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 8; j++)
                        temp[j] = A[m * 8 + i][n * 8 + j];
                    for (j = 0; j < 4; j++)
                        B[n * 8 + j][m * 8 + i] = temp[j];
                    for (j = 0; j < 4; j++)
                        B[n * 8 + j][m * 8 + i + 4] = temp[j + 4];
                }
                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 4; j++)
                        temp[j + 4] = A[m * 8 + j + 4][n * 8 + i];
                    for (j = 0; j < 4; j++)
                        temp[j] = B[n * 8 + i][m * 8 + j + 4];
                    for (j = 0; j < 4; j++)
                        B[n * 8 + i][m * 8 + j + 4] = temp[j + 4];
                    for (j = 0; j < 4; j++)
                        B[n * 8 + i + 4][m * 8 + j] = temp[j];
                    for (j = 0; j < 4; j++)
                    {
                        if (i == j)
                            continue;
                        else
                            B[n * 8 + i + 4][m * 8 + j + 4] = A[m * 8 + j + 4][n * 8 + i + 4];
                    }
                }
                for (i = 0; i < 4; i++)
                    temp[i] = A[m * 8 + i + 4][n * 8 + i + 4];
                for (i = 0; i < 4; i++)
                    B[n * 8 + i + 4][m * 8 + i + 4] = temp[i];
            }
        }
    }
    else
    {
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < M; j++)
            {
                B[j][i] = A[i][j];
            }
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    registerTransFunction(trans_test, trans_desc_test);

    /* Register any additional transpose functions */
    //registerTransFunction(trans, trans_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
