/**
 *
 * @file
 *
 *  hw is a software package provided by:
 *  University of Tennessee, US,
 *  University of Manchester, UK.
 *
 * @generated from /home/luszczek/workspace/hw/bitbucket/hw/include/hw_core_blas_z.h, normal z -> s, Fri Sep 28 17:38:01 2018
 *
 **/
#ifndef hw_CORE_BLAS_S_H
#define hw_CORE_BLAS_S_H

#include "hw_async.h"
#include "hw_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define REAL

/******************************************************************************/
#ifdef COMPLEX
float fabsf(float alpha);
#endif

void hw_core_omp_slacpy(char uper_lower, char transa,
                     int m, int n,
                     const float *A, int lda,
                           float *B, int ldb,
                     hw_sequence_t *sequence, hw_request_t *request);


void hw_core_slacpy(char uper_lower, char transa,
                 int m, int n,
                 const float *A, int lda,
                       float *B, int ldb);
#undef REAL

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // hw_CORE_BLAS_S_H
