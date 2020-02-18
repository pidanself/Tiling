/**
 *
 * @file
 *
 *  hw is a software package provided by:
 *  University of Tennessee, US,
 *  University of Manchester, UK.
 *
 **/
#ifndef hw_DESCRIPTOR_H
#define hw_DESCRIPTOR_H

#include "hw_types.h"
#include "hw_error.h"

#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @ingroup hw_descriptor
 *
 * Tile matrix descriptor.
 *
 *              n1      n2
 *         +----------+---+
 *         |          |   |    m1 = lm - (lm%mb)
 *         |          |   |    m2 = lm%mb
 *     m1  |    A11   |A12|    n1 = ln - (ln%nb)
 *         |          |   |    n2 = ln%nb
 *         |          |   |
 *         +----------+---+
 *     m2  |    A21   |A22|
 *         +----------+---+
 *
 **/
typedef struct {
    // pointer and offsets
    void *matrix; ///< pointer to the beginning of the matrix 改成p_data

    //改成short\char
    hw_enum_t type;      ///< general, general band, etc. =general  改成matrix_shape
    hw_enum_t uplo;      ///< upper, lower, etc. = 改成uper_lower
    hw_enum_t precision; ///< precision of the matrix =hwRealFloat 改成data_type


    //改成位置的英文的变量名
    size_t A21;   ///< pointer to the beginning of A21 =
    size_t A12;   ///< pointer to the beginning of A12
    size_t A22;   ///< pointer to the beginning of A22

    // tile parameters
    int mb; ///< number of rows in a tile =自己定一个 改成rows_per_chunk
    int nb; ///< number of columns in a tile =自己定一个

    //让lm=mb ln=nb试试
    // main matrix parameters
    int gm;  ///< number of rows of the entire matrix =自己定一个 total_rows
    int gn;  ///< number of columns of the entire matrix =自己定一个
    int gmt; ///< number of tile rows of the entire matrix  total_chunk_rows
    int gnt; ///< number of tile columns of the entire matrix

    // submatrix parameters
    int i;  ///< row index to the beginning of the submatrix
    int j;  ///< column index to the beginning of the submatrix
    int m;  ///< number of rows of the submatrix
    int n;  ///< number of columns of the submatrix
    int mt; ///< number of tile rows of the submatrix
    int nt; ///< number of tile columns of the submatrix

    // submatrix parameters for a band matrix
    int kl;  ///< number of rows below the diagonal
    int ku;  ///< number of rows above the diagonal
    int klt; ///< number of tile rows below the diagonal tile
    int kut; ///< number of tile rows above the diagonal tile
             ///  includes the space for potential fills, i.e., kl+ku
} hw_desc_t;//用到

/******************************************************************************/
static inline size_t hw_element_size(int type)
{
    switch (type) {
    case hwByte:          return          1;
    case hwInteger:       return   sizeof(int);
    case hwRealFloat:     return   sizeof(float);
    case hwRealDouble:    return   sizeof(double);
    case hwComplexFloat:  return 2*sizeof(float);
    case hwComplexDouble: return 2*sizeof(double);
    default: assert(0);
    }
}//用到

/******************************************************************************/
static inline void *hw_tile_addr_general(hw_desc_t A, int m, int n)
{
    int mm = m + A.i/A.mb;
    int nn = n + A.j/A.nb;
    size_t eltsize = hw_element_size(A.precision);
    size_t offset = 0;

    int lm1 = A.gm/A.mb;
    int ln1 = A.gn/A.nb;

    if (mm < lm1)
        if (nn < ln1)
            offset = A.mb*A.nb*(mm + (size_t)lm1 * nn);
        else
            offset = A.A12 + ((size_t)A.mb * (A.gn%A.nb) * mm);
    else
        if (nn < ln1)
            offset = A.A21 + ((size_t)A.nb * (A.gm%A.mb) * nn);
        else
            offset = A.A22;

    return (void*)((char*)A.matrix + (offset*eltsize));
}//用到

/******************************************************************************/
static inline void *hw_tile_addr_triangle(hw_desc_t A, int m, int n)
{
    int mm = m + A.i/A.mb;
    int nn = n + A.j/A.nb;
    size_t eltsize = hw_element_size(A.precision);
    size_t offset = 0;

    int lm1 = A.gm/A.mb;
    int ln1 = A.gn/A.nb;

    if (mm < lm1) {
        if (nn < ln1) {
            if (A.type == hwUpper) {
                offset = A.mb*A.nb*(mm + (nn * (nn + 1))/2);
            }
            else {
                offset = A.mb*A.nb*((mm - nn) + (nn * (2*lm1 - (nn-1)))/2);
            }
        }
        else {
            offset = A.A12 + ((size_t)A.mb * (A.gn%A.nb) * mm);
        }
    }
    else {
        if (nn < ln1) {
            offset = A.A21 + ((size_t)A.nb * (A.gm%A.mb) * nn);
        }
        else {
            offset = A.A22;
        }
    }

    return (void*)((char*)A.matrix + (offset*eltsize));
}//用到

/******************************************************************************/
static inline void *hw_tile_addr_general_band(hw_desc_t A, int m, int n)
{
    return hw_tile_addr_general(A, (A.kut-1)+m-n, n);
}

/******************************************************************************/
static inline void *hw_tile_addr(hw_desc_t A, int m, int n)
{
    if (A.type == hwGeneral) {
        return hw_tile_addr_general(A, m, n);
    }
    else if (A.type == hwGeneralBand) {
        return hw_tile_addr_general_band(A, m, n);
    }
    else if (A.type == hwUpper || A.type == hwLower) {
        return hw_tile_addr_triangle(A, m, n);
    }
    else {
        hw_fatal_error("invalid matrix type");
        return NULL;
    }
}

/***************************************************************************//**
 *
 *  Returns the height of the tile with vertical position k.
 *
 */
static inline int hw_tile_mmain(hw_desc_t A, int k)
{
    if (A.type == hwGeneralBand) {
        return A.mb;
    }
    else {
        if (A.i/A.mb+k < A.gm/A.mb)
            return A.mb;
        else
            return A.gm%A.mb;
    }
}

/***************************************************************************//**
 *
 *  Returns the width of the tile with horizontal position k.
 *
 */
static inline int hw_tile_nmain(hw_desc_t A, int k)
{
    if (A.j/A.nb+k < A.gn/A.nb)
        return A.nb;
    else
        return A.gn%A.nb;
}

/***************************************************************************//**
 *
 *  Returns the height of the portion of the submatrix occupying the tile
 *  at vertical position k.
 *
 */
static inline int hw_tile_mview(hw_desc_t A, int k)
{
    if (k < A.mt-1)
        return A.mb;
    else
        if ((A.i+A.m)%A.mb == 0)
            return A.mb;
        else
            return (A.i+A.m)%A.mb;
}

/***************************************************************************//**
 *
 *  Returns the width of the portion of the submatrix occupying the tile
 *  at horizontal position k.
 *
 */
static inline int hw_tile_nview(hw_desc_t A, int k)
{
    if (k < A.nt-1)
        return A.nb;
    else
        if ((A.j+A.n)%A.nb == 0)
            return A.nb;
        else
            return (A.j+A.n)%A.nb;
}

/******************************************************************************/
static inline int hw_tile_mmain_band(hw_desc_t A, int m, int n)
{
    return hw_tile_mmain(A, (A.kut-1)+m-n);
}

/******************************************************************************/
int hw_desc_general_create(hw_enum_t dtyp, int mb, int nb,
                               int lm, int ln, int i, int j, int m, int n,
                               hw_desc_t *A);

int hw_desc_general_band_create(hw_enum_t dtyp, hw_enum_t uplo,
                                    int mb, int nb, int lm, int ln,
                                    int i, int j, int m, int n, int kl, int ku,
                                    hw_desc_t *A);

int hw_desc_triangular_create(hw_enum_t dtyp, hw_enum_t uplo, int mb, int nb,
                                  int lm, int ln, int i, int j, int m, int n,
                                  hw_desc_t *A);

int hw_desc_destroy(hw_desc_t *A);

int hw_desc_general_init(hw_enum_t precision, void *matrix,
                             int mb, int nb, int lm, int ln, int i, int j,
                             int m, int n, hw_desc_t *A);

int hw_desc_general_band_init(hw_enum_t precision, hw_enum_t uplo,
                                  void *matrix, int mb, int nb, int lm, int ln,
                                  int i, int j, int m, int n, int kl, int ku,
                                  hw_desc_t *A);

int hw_desc_triangular_init(hw_enum_t precision, hw_enum_t uplo, void *matrix,
                                int mb, int nb, int lm, int ln, int i, int j,
                                int m, int n, hw_desc_t *A);

int hw_desc_check(hw_desc_t A);
int hw_desc_general_check(hw_desc_t A);
int hw_desc_general_band_check(hw_desc_t A);

hw_desc_t hw_desc_view(hw_desc_t A, int i, int j, int m, int n);

int hw_descT_create(hw_desc_t A, int ib, hw_enum_t householder_mode,
                        hw_desc_t *T);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // hw_DESCRIPTOR_H