/**
 *
 * @file
 *
 *  hw is a software package provided by:
 *  University of Tennessee, US,
 *  University of Manchester, UK.
 *
 **/
#ifndef hw_TYPES_H
#define hw_TYPES_H

#include <complex.h>

/*
 * RELEASE is a, b, c, f (alpha, beta, candidate, and final).
 */
#define hw_VERSION_MAJOR   19
#define hw_VERSION_MINOR   8
#define hw_VERSION_PATCH   1

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#if defined(HAVE_MKL) || defined(hw_WITH_MKL)
#define lapack_complex_float hw_complex32_t
#define lapack_complex_double hw_complex64_t
#endif

/***************************************************************************//**
 *
 *  Some CBLAS routines take scalars by value in real arithmetic
 *  and by pointer in complex arithmetic.
 *  In precision generation, CBLAS_SADDR is removed from real arithmetic files.
 *
 **/
#ifndef CBLAS_SADDR
#if defined(hw_WITH_OPENBLAS)
#define CBLAS_SADDR(var) ((void*)&(var))
#else
#define CBLAS_SADDR(var) &(var)
#endif
#endif

/******************************************************************************/
enum {
    hwByte          = '0',
    hwInteger       = '1',
    hwRealFloat     = '2',
    hwRealDouble    = '3',
    hwComplexFloat  = '4',
    hwComplexDouble = '5'
};

/***************************************************************************//**
 *
 *  hw constants - CBLAS & LAPACK.
 *  The naming and numbering is consistent with:
 *
 *    - CBLAS - http://www.netlib.org/blas/blast-forum/cblas.tgz,
 *    - LAPACKE - http://www.netlib.org/lapack/lapwrapc/.
 *
 *  During precision generation, hw_ConjTrans is conveted to hwTrans,
 *  while hwConjTrans is preserved.
 *
 **/
enum {
    hwInvalid       = -1,

    hwNoTrans       = '4',
    hwTrans         = '5',
    hwConjTrans     = '6',
    hw_ConjTrans    = hwConjTrans,

    hwUpper         = '0', //121
    hwLower         = '1',
    hwGeneral       = '2',
    hwGeneralBand   = '3', //124

    hwNonUnit       = 131,
    hwUnit          = 132,

    hwLeft          = 141,
    hwRight         = 142,

    hwOneNorm       = 171,
    hwRealOneNorm   = 172,
    hwTwoNorm       = 173,
    hwFrobeniusNorm = 174,
    hwInfNorm       = 175,
    hwRealInfNorm   = 176,
    hwMaxNorm       = 177,
    hwRealMaxNorm   = 178,

    hwForward       = 391,
    hwBackward      = 392,

    hwColumnwise    = 401,
    hwRowwise       = 402,

    hwW             = 501,
    hwA2            = 502
};

enum {
    hwSuccess = 0,
    hwErrorNotInitialized,
    hwErrorNotSupported,
    hwErrorIllegalValue,
    hwErrorOutOfMemory,
    hwErrorNullParameter,
    hwErrorInternal,
    hwErrorSequence,
    hwErrorComponent,
    hwErrorEnvironment
};

enum {
    hwInplace,
    hwOutplace
};

enum {
    hwFlatHouseholder,
    hwTreeHouseholder
};

enum {
    hwDisabled = 0,
    hwEnabled = 1
};

enum {
    hwTuning,
    hwNb,
    hwIb,
    hwInplaceOutplace,
    hwNumPanelThreads,
    hwHouseholderMode
};

/******************************************************************************/
typedef int hw_enum_t;

typedef float  _Complex hw_complex32_t;
typedef double _Complex hw_complex64_t;

/******************************************************************************/
hw_enum_t hw_diag_const(char lapack_char);
hw_enum_t hw_direct_const(char lapack_char);
hw_enum_t hw_norm_const(char lapack_char);
hw_enum_t hw_side_const(char lapack_char);
hw_enum_t hw_storev_const(char lapack_char);
hw_enum_t hw_trans_const(char lapack_char);
hw_enum_t hw_uplo_const(char lapack_char);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // hw_TYPES_H
