/**
 *
 * @file
 *
 *  hw is a software package provided by:
 *  University of Tennessee, US,
 *  University of Manchester, UK.
 *
 * @generated from /home/luszczek/workspace/hw/bitbucket/hw/include/core_lapack_z.h, normal z -> s, Fri Sep 28 17:38:00 2018
 *
 **/

#ifndef hw_CORE_LAPACK_S_H
#define hw_CORE_LAPACK_S_H

#ifdef __cplusplus
extern "C" {
#endif

// LAPACK_GLOBAL is Fortran name mangling macro from LAPACKE

// LAPACKE_slantr broken (returns 0) in LAPACKE < 3.6.1
#ifndef LAPACK_slantr
#define LAPACK_slantr LAPACK_GLOBAL(slantr, SLANTR)
float LAPACK_slantr(const char *norm, const char *uplo, const char *diag,
                     const lapack_int *m, const lapack_int *n,
                     const float *A, const lapack_int *lda,
                     float *work);
#endif

// LAPACKE_slascl not available in LAPACKE < 3.6.0
#ifndef LAPACK_slascl
#define LAPACK_slascl LAPACK_GLOBAL(slascl, SLASCL)
void LAPACK_slascl(const char *type, const lapack_int *kl, const lapack_int *ku,
                   const float *cfrom, const float *cto,
                   const lapack_int *m, const lapack_int *n,
                   float *A, const lapack_int *lda,
                   lapack_int *info);
#endif

// LAPACKE_slassq not available yet
#ifndef LAPACK_slassq
#define LAPACK_slassq LAPACK_GLOBAL(slassq, SLASSQ)
void LAPACK_slassq(const lapack_int *n, const float *x, const lapack_int *incx,
                   float *scale, float *sumsq);
#endif

// LAPACKE_slangb not available yet
#ifndef LAPACK_slangb
#define LAPACK_slangb LAPACK_GLOBAL(slangb, SLANGB)
float LAPACK_slangb(const char *norm,
                     const lapack_int *n, const lapack_int *kl, const lapack_int *ku,
                     const float *A, const lapack_int *lda,
                     float *work);

#endif

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // hw_CORE_LAPACK_S_H
