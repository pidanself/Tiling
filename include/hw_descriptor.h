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
 *         |          |   |    m1 = lm - (lm%rows_per_chunk)
 *         |          |   |    m2 = lm%rows_per_chunk
 *     m1  |    A11   |A12|    n1 = ln - (ln%cols_per_chunk)
 *         |          |   |    n2 = ln%cols_per_chunk
 *         |          |   |
 *         +----------+---+
 *     m2  |    A21   |A22|
 *         +----------+---+
 *
 **/
typedef struct {
    void *p_data; ///< pointer to the beginning of the matrix matrix
    char matrix_shape;      ///< general, general band, etc.   type
    char uper_lower;      ///< upper, lower, etc.  uplo
    char data_type; ///< data_type of the matrix precision

    size_t lower_left;   ///< pointer to the beginning of A21 
    size_t upper_right;   ///< pointer to the beginning of A12
    size_t lower_right;   ///< pointer to the beginning of A22

    // tile parameters
    int rows_per_chunk; ///< number of rows in a tile mb
    int cols_per_chunk; ///< number of columns in a tile nb

    // main matrix parameters
    int total_rows;  ///< number of rows of the entire matrix gm 
    int total_cols;  ///< number of columns of the entire matrix gn
    int total_chunk_rows; ///< number of tile rows of the entire matrix  gmt
    int total_chunk_cols; ///< number of tile columns of the entire matrix gnt

    int m;  ///< number of rows of the submatrix
    int n;  ///< number of columns of the submatrix
    int mt; ///< number of tile rows of the submatrix
    int nt; ///< number of tile columns of the submatrix

} hw_desc_t;//用到

/******************************************************************************/
static inline size_t hw_element_size(char matrix_shape)
{
    switch (matrix_shape) {
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
static inline void *hw_tile_addr_general(hw_desc_t A, int m, int n,int i,int j)
{
    //一般情况下i与j为0
    int mm = m + i/A.rows_per_chunk;
    int nn = n + j/A.cols_per_chunk;
    size_t eltsize = hw_element_size(A.data_type);
    size_t offset = 0;

    int lm1 = A.total_rows/A.rows_per_chunk;
    int ln1 = A.total_cols/A.cols_per_chunk;

    if (mm < lm1)
        if (nn < ln1)
            offset = A.rows_per_chunk*A.cols_per_chunk*(mm + (size_t)lm1 * nn);
        else
            offset = A.upper_right + ((size_t)A.rows_per_chunk * (A.total_cols%A.cols_per_chunk) * mm);
    else
        if (nn < ln1)
            offset = A.lower_left + ((size_t)A.cols_per_chunk * (A.total_rows%A.rows_per_chunk) * nn);
        else
            offset = A.lower_right;

    return (void*)((char*)A.p_data + (offset*eltsize));
}//用到

/******************************************************************************/
static inline void *hw_tile_addr_triangle(hw_desc_t A, int m, int n,int i,int j)
{
    int mm = m + i/A.rows_per_chunk;
    int nn = n + j/A.cols_per_chunk;
    size_t eltsize = hw_element_size(A.data_type);
    size_t offset = 0;

    int lm1 = A.total_rows/A.rows_per_chunk;
    int ln1 = A.total_cols/A.cols_per_chunk;

    if (mm < lm1) {
        if (nn < ln1) {
            if (A.matrix_shape == hwUpper) {
                offset = A.rows_per_chunk*A.cols_per_chunk*(mm + (nn * (nn + 1))/2);
            }
            else {
                offset = A.rows_per_chunk*A.cols_per_chunk*((mm - nn) + (nn * (2*lm1 - (nn-1)))/2);
            }
        }
        else {
            offset = A.upper_right + ((size_t)A.rows_per_chunk * (A.total_cols%A.cols_per_chunk) * mm);
        }
    }
    else {
        if (nn < ln1) {
            offset = A.lower_left + ((size_t)A.cols_per_chunk * (A.total_rows%A.rows_per_chunk) * nn);
        }
        else {
            offset = A.lower_right;
        }
    }

    return (void*)((char*)A.p_data + (offset*eltsize));
}//用到



/******************************************************************************/
static inline void *hw_tile_addr(hw_desc_t A, int m, int n,int i,int j)
{
    if (A.matrix_shape == hwGeneral) {
        return hw_tile_addr_general(A, m, n,i,j);
    }
    else if (A.matrix_shape == hwGeneralBand) {
        //return hw_tile_addr_general_band(A, m, n);
        return NULL;
    }
    else if (A.matrix_shape == hwUpper || A.matrix_shape == hwLower) {
        return hw_tile_addr_triangle(A, m, n,i,j);
    }
    else {
        hw_fatal_error("invalid data matrix_shape");
        return NULL;
    }
}

/***************************************************************************//**
 *
 *  Returns the height of the tile with vertical position k.
 *
 */
static inline int hw_tile_mmain(hw_desc_t A, int k,int i)
{
    if (A.matrix_shape == hwGeneralBand) {
        return A.rows_per_chunk;
    }
    else {
        if (i/A.rows_per_chunk+k < A.total_rows/A.rows_per_chunk)
            return A.rows_per_chunk;
        else
            return A.total_rows%A.rows_per_chunk;
    }
}

/***************************************************************************//**
 *
 *  Returns the width of the tile with horizontal position k.
 *
 */
static inline int hw_tile_nmain(hw_desc_t A, int k,int j)
{
    if (j/A.cols_per_chunk+k < A.total_cols/A.cols_per_chunk)
        return A.cols_per_chunk;
    else
        return A.total_cols%A.cols_per_chunk;
}

/***************************************************************************//**
 *
 *  Returns the height of the portion of the submatrix occupying the tile
 *  at vertical position k.
 *
 */
static inline int hw_tile_mview(hw_desc_t A, int k,int i)
{
    if (k < A.total_chunk_rows-1)
        return A.rows_per_chunk;
    else
        if ((i+A.total_rows)%A.rows_per_chunk == 0)
            return A.rows_per_chunk;
        else
            return (i+A.total_rows)%A.rows_per_chunk;
}

/***************************************************************************//**
 *
 *  Returns the width of the portion of the submatrix occupying the tile
 *  at horizontal position k.
 *
 */
static inline int hw_tile_nview(hw_desc_t A, int k,int j)
{
    if (k < A.total_chunk_cols-1)
        return A.cols_per_chunk;
    else
        if ((j+A.total_cols)%A.cols_per_chunk == 0)
            return A.cols_per_chunk;
        else
            return (j+A.total_cols)%A.cols_per_chunk;
}

/******************************************************************************/
int hw_desc_general_create(char data_type, int rows_per_chunk, int cols_per_chunk,
                               int lm, int ln,
                               hw_desc_t *A);

int hw_desc_general_band_create(hw_enum_t dtyp, char uper_lower,
                                    int rows_per_chunk, int cols_per_chunk, int lm, int ln,
                                    int i, int j, int m, int n, int kl, int ku,
                                    hw_desc_t *A);

int hw_desc_triangular_create(hw_enum_t dtyp, char uper_lower, int rows_per_chunk, int cols_per_chunk,
                                  int lm, int ln, int i, int j, int m, int n,
                                  hw_desc_t *A);

int hw_desc_destroy(hw_desc_t *A);

int hw_desc_general_init(char data_type, void *matrix,
                             int rows_per_chunk, int cols_per_chunk, int lm, int ln,
                             hw_desc_t *A);

int hw_desc_general_band_init(char data_type, char uper_lower,
                                  void *matrix, int rows_per_chunk, int cols_per_chunk, int lm, int ln,
                                  int i, int j, int m, int n, int kl, int ku,
                                  hw_desc_t *A);

int hw_desc_triangular_init(char data_type, char uper_lower, void *matrix,
                                int rows_per_chunk, int cols_per_chunk, int lm, int ln, int i, int j,
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