/**
 *
 * @file
 *
 *  hw is a software package provided by:
 *  University of Tennessee, US,
 *  University of Manchester, UK.
 *
 * @generated from /home/luszczek/workspace/hw/bitbucket/hw/core_blas/core_zlacpy.c, normal z -> s, Fri Sep 28 17:38:19 2018
 *
 **/

#include "../include/hw_core_blas.h"
#include "../include/hw_types.h"
#include "../include/core_lapack.h"
#include "../include/hw_internal.h"

/***************************************************************************//**
 *
 * @ingroup core_lacpy
 *
 *  Copies all or part of a two-dimensional matrix A to another matrix B.
 *
 *******************************************************************************
 *
 * @param[in] uplo
 *          - hwGeneral: entire A,
 *          - hwUpper:   upper triangle,
 *          - hwLower:   lower triangle.
 *
 * @param[in] transa
 *          - hwNoTrans:   A is not transposed,
 *          - hwTrans:     A is transposed,
 *          - hwConjTrans: A is conjugate transposed.
 *
 * @param[in] m
 *          The number of rows of the matrices A and B.
 *          m >= 0.
 *
 * @param[in] n
 *          The number of columns of the matrices A and B.
 *          n >= 0.
 *
 * @param[in] A
 *          The m-by-n matrix to copy.
 *
 * @param[in] lda
 *          The leading dimension of the array A.
 *          lda >= max(1,m).
 *
 * @param[out] B
 *          The m-by-n copy of the matrix A.
 *          On exit, B = A ONLY in the locations specified by uper_lower.
 *
 * @param[in] ldb
 *          The leading dimension of the array B.
 *          ldb >= max(1,m).
 *
 ******************************************************************************/
__attribute__((weak))
void hw_core_slacpy(char uper_lower, char transa,
                 int m, int n,
                 const float *A, int lda,
                       float *B, int ldb)
{
    if (transa == hwNoTrans) {
        LAPACKE_slacpy_work(LAPACK_COL_MAJOR,
                            lapack_const(uper_lower),
                            m, n,
                            A, lda,
                            B, ldb);
    }
    else if (transa == hwTrans) {
        switch (uper_lower) {
        case hwUpper:
            for (int i = 0; i < imin(m, n); i++)
                for (int j = i; j < n; j++)
                    B[j + i*ldb] = A[i + j*lda];
            break;
        case hwLower:
            for (int i = 0; i < m; i++)
                for (int j = 0; j <= imin(i, n); j++)
                    B[j + i*ldb] = A[i + j*lda];
            break;
        case hwGeneral:
            for (int i = 0; i < m; i++)
                for (int j = 0; j < n; j++)
                    B[j + i*ldb] = A[i + j*lda];
            break;
        }
    }
    else {
        switch (uper_lower) {
        case hwUpper:
            for (int i = 0; i < imin(m, n); i++)
                for (int j = i; j < n; j++)
                    B[j + i*ldb] = (A[i + j*lda]);
            break;
        case hwLower:
            for (int i = 0; i < m; i++)
                for (int j = 0; j <= imin(i, n); j++)
                    B[j + i*ldb] = (A[i + j*lda]);
            break;
        case hwGeneral:
            for (int i = 0; i < m; i++)
                for (int j = 0; j < n; j++)
                    B[j + i*ldb] = (A[i + j*lda]);
            break;
        }
    }
}

/******************************************************************************/
void hw_core_omp_slacpy(char uper_lower, char transa,
                     int m, int n,
                     const float *A, int lda,
                           float *B, int ldb,
                     hw_sequence_t *sequence, hw_request_t *request)
{
    //暂时先不考虑并行的事情
    // #pragma omp task depend(in:A[0:lda*n]) \
    //                  depend(out:B[0:ldb*n])
    {
        if (sequence->status == hwSuccess)
            hw_core_slacpy(uper_lower, transa,
                        m, n,
                        A, lda,
                        B, ldb);
    }
}
