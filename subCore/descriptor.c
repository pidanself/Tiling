#include "../include/hw_types.h"
#include "../include/hw_descriptor.h"
#include "../include/hw_context.h"


int hw_desc_general_create(hw_enum_t precision, int mb, int nb,
                               int lm, int ln, int i, int j, int m, int n,
                               hw_desc_t *A)
{
    hw_context_t *hw = hw_context_self();
    if (hw == NULL) {
        hw_error("hw not initialized");
        return hwErrorNotInitialized;
    }
    // Initialize the descriptor.
    int retval = hw_desc_general_init(precision, NULL, mb, nb,
                                          lm, ln, i, j, m, n, A);
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
    size_t size = (size_t)A->gm*A->gn*
                  hw_element_size(A->precision);
    A->matrix = malloc(size);
    if (A->matrix == NULL) {
        hw_error("malloc() failed");
        return hwErrorOutOfMemory;
    }
    return hwSuccess;
}

int hw_desc_general_init(hw_enum_t precision, void *matrix,
                             int mb, int nb, int lm, int ln, int i, int j,
                             int m, int n, hw_desc_t *A)
{
    // type and precision
    A->type = hwGeneral;
    A->precision = precision;

    // pointer and offsets
    A->matrix = matrix;
    A->A21 = (size_t)(lm - lm%mb) * (ln - ln%nb);
    A->A12 = (size_t)(     lm%mb) * (ln - ln%nb) + A->A21;
    A->A22 = (size_t)(lm - lm%mb) * (     ln%nb) + A->A12;

    // tile parameters
    A->mb = mb;
    A->nb = nb;

    // main matrix parameters
    A->gm = lm;
    A->gn = ln;

    A->gmt = (lm%mb == 0) ? (lm/mb) : (lm/mb+1);
    A->gnt = (ln%nb == 0) ? (ln/nb) : (ln/nb+1);

    // submatrix parameters
    A->i = i;
    A->j = j;
    A->m = m;
    A->n = n;

    A->mt = (m == 0) ? 0 : (i+m-1)/mb - i/mb + 1;
    A->nt = (n == 0) ? 0 : (j+n-1)/nb - j/nb + 1;

    // band parameters
    A->kl = m-1;
    A->ku = n-1;
    A->klt = A->mt;
    A->kut = A->nt;

    return hwSuccess;
}

int hw_desc_check(hw_desc_t A)
{
    if (A.type == hwGeneral ||
        A.type == hwUpper ||
        A.type == hwLower) {
        return hw_desc_general_check(A);
    }
    else if (A.type == hwGeneralBand) {
        //暂时不涉及
        //return hw_desc_general_band_check(A);
    }
    else {
        hw_error("invalid matrix type");
        return hwErrorIllegalValue;
    }
}

int hw_desc_general_check(hw_desc_t A)
{
    if (A.precision != hwRealFloat &&
        A.precision != hwRealDouble &&
        A.precision != hwComplexFloat &&
        A.precision != hwComplexDouble  ) {
        hw_error("invalid matrix type");
        return hwErrorIllegalValue;
    }
    if (A.mb <= 0 || A.nb <= 0) {
        hw_error("negative tile dimension");
        return hwErrorIllegalValue;
    }
    if ((A.m < 0) || (A.n < 0)) {
        hw_error("negative matrix dimension");
        return hwErrorIllegalValue;
    }
    if ((A.gm < A.m) || (A.gn < A.n)) {
        hw_error("invalid leading dimensions");
        return hwErrorIllegalValue;
    }
    if ((A.i > 0 && A.i >= A.gm) ||
        (A.j > 0 && A.j >= A.gn)) {
        hw_error("beginning of the matrix out of bounds");
        return hwErrorIllegalValue;
    }
    if (A.i+A.m > A.gm || A.j+A.n > A.gn) {
        hw_error("submatrix out of bounds");
        return hwErrorIllegalValue;
    }
    if ((A.i % A.mb != 0) || (A.j % A.nb != 0)) {
        hw_error("submatrix not aligned to a tile");
        return hwErrorIllegalValue;
    }
    return hwSuccess;
}