/**
 *
 * @file
 *
 *  hw is a software package provided by:
 *  University of Tennessee, US,
 *  University of Manchester, UK.
 *
 **/
#ifndef hw_CORE_BLAS_H
#define hw_CORE_BLAS_H

#include <stdio.h>
#include "hw_types.h"

#ifdef __cplusplus
extern "C" {
#endif

static const char *lapack_constants[] = {
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",

    "", "", "", "", "", "", "", "", "", "",
    "",
    "NoTrans",                              ///< 111: hwNoTrans
    "Trans",                                ///< 112: hwTrans
    "ConjTrans",                            ///< 113: hwConjTrans

    "", "", "", "", "", "", "",
    "Upper",                                ///< 121: hwUpper
    "Lower",                                ///< 122: hwLower
    "General",                              ///< 123: hwGeneral

    "", "", "", "", "", "", "",
    "NonUnit",                              ///< 131: hwNonUnit
    "Unit",                                 ///< 132: hwUnit

    "", "", "", "", "", "", "", "",
    "Left",                                 ///< 141: hwLeft
    "Right",                                ///< 142: hwRight

    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "",
    "One",                                  ///< 171: hwOneNorm
    "",                                     ///< 172: hwRealOneNorm
    "Two",                                  ///< 173: hwTwoNorm
    "Frobenius",                            ///< 174: hwFrobeniusNorm
    "Infinity",                             ///< 175: hwInfNorm
    "",                                     ///< 176: hwRealInfNorm
    "Maximum",                              ///< 177: hwMaxNorm
    "",                                     ///< 178: hwRealMaxNorm

    "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "",
    "Forward",                             ///< 391: hwForward
    "Backward",                            ///< 392: hwBackward
    "", "", "", "", "", "", "", "",
    "Columnwise",                          ///< 401: hwColumnwise
    "Rowwise"                              ///< 402: hwRowwise
};



/***************************************************************************//**
 * @retval LAPACK character constant corresponding to hw constant
 * @ingroup hw_const
 ******************************************************************************/
static inline char lapack_const(char hw_const) {
    if(hw_const==hwUpper){
        return lapack_constants[121][0];
    }
    else if(hw_const==hwLower){
        return lapack_constants[122][0];
    }
    else{
        return lapack_constants[123][0];
    }
}

#define hw_coreblas_error(msg) \
        hw_coreblas_error_func_line_file(__func__, __LINE__, __FILE__, msg)

static inline void hw_coreblas_error_func_line_file(
    char const *func, int line, const char *file, const char *msg)
{
    fprintf(stderr,
            "COREBLAS ERROR at %d of %s() in %s: %s\n",
            line, func, file, msg);
}

#ifdef __cplusplus
}  // extern "C"
#endif

#include "hw_core_blas_s.h"

#endif // hw_CORE_BLAS_H
