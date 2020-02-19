#include "../include/hw_types.h"
#include "../include/hw_descriptor.h"
#include "../include/hw_context.h"


int hw_desc_general_create(char data_type, int rows_per_chunk, int cols_per_chunk,
                               int lm, int ln,
                               hw_desc_t *A)
{
    hw_context_t *hw = hw_context_self();
    if (hw == NULL) {
        hw_error("hw not initialized");
        return hwErrorNotInitialized;
    }
    // Initialize the descriptor.
    int retval = hw_desc_general_init(data_type, NULL, rows_per_chunk, cols_per_chunk,
                                          lm, ln, A);
    if (retval != hwSuccess) {
        hw_error("hw_desc_general_init() failed");
        return retval;
    }
    // Check the descriptor.
    retval = hw_desc_check(*A);
    if (retval != hwSuccess) {
        hw_error("hw_desc_check() failed");
        return hwErrorIllegalValue;
    }
    // Allocate the matrix.
    size_t size = (size_t)A->total_rows*A->total_cols*
                  hw_element_size(A->data_type);
    A->p_data = malloc(size);
    if (A->p_data == NULL) {
        hw_error("malloc() failed");
        return hwErrorOutOfMemory;
    }
    return hwSuccess;
}

int hw_desc_general_init(char data_type, void *matrix,
                             int rows_per_chunk, int cols_per_chunk, int lm, int ln, hw_desc_t *A)
{
    // matrix_shape and data_type
    A->matrix_shape = hwGeneral;
    A->data_type = data_type;

    // pointer and offsets
    A->p_data = matrix;
    A->lower_left = (size_t)(lm - lm%rows_per_chunk) * (ln - ln%cols_per_chunk);
    A->upper_right = (size_t)(     lm%rows_per_chunk) * (ln - ln%cols_per_chunk) + A->lower_left;
    A->lower_right = (size_t)(lm - lm%rows_per_chunk) * (     ln%cols_per_chunk) + A->upper_right;

    // tile parameters
    A->rows_per_chunk = rows_per_chunk;
    A->cols_per_chunk = cols_per_chunk;

    // main matrix parameters
    A->total_rows = lm;
    A->total_cols = ln;

    A->total_chunk_rows = (lm%rows_per_chunk == 0) ? (lm/rows_per_chunk) : (lm/rows_per_chunk+1);
    A->total_chunk_cols = (ln%cols_per_chunk == 0) ? (ln/cols_per_chunk) : (ln/cols_per_chunk+1);

    A->m = lm;
    A->n = ln;

    A->mt = A->total_chunk_rows;
    A->nt = A->total_chunk_cols;

    return hwSuccess;
}

int hw_desc_check(hw_desc_t A)
{
    if (A.matrix_shape == hwGeneral ||
        A.matrix_shape == hwUpper ||
        A.matrix_shape == hwLower) {
        return hw_desc_general_check(A);
    }
    else if (A.matrix_shape == hwGeneralBand) {
        //暂时不涉及
        //return hw_desc_general_band_check(A);
    }
    else {
        hw_error("invalid data matrix_shape");
        return hwErrorIllegalValue;
    }
}

int hw_desc_general_check(hw_desc_t A)
{
    if (A.data_type != hwRealFloat &&
        A.data_type != hwRealDouble &&
        A.data_type != hwComplexFloat &&
        A.data_type != hwComplexDouble  ) {
        hw_error("invalid data matrix_shape");
        return hwErrorIllegalValue;
    }
    if (A.rows_per_chunk <= 0 || A.cols_per_chunk <= 0) {
        hw_error("negative tile dimension");
        return hwErrorIllegalValue;
    }
    return hwSuccess;
}