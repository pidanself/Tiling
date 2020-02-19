#include "../include/hw_descriptor.h"
#include "../include/hw_async.h"
#include "../include/hw_types.h"
#include "../include/hw_core_blas.h"
#include "../include/hw.h"

void hw_ge2desc(float *pA, int lda,
                      hw_desc_t A,
                      hw_sequence_t *sequence,
                      hw_request_t *request)
{
    // Return if failed sequence.
    if (sequence->status != hwSuccess)
        return;

    float *f77;
    float *bdl;

    int x1, y1;
    int x2, y2;
    int n, m, ldt;

    for (m = 0; m < A.mt; m++) {
        ldt = hw_tile_mmain(A, m,0);
        for (n = 0; n < A.nt; n++) {
            x1 = 0;
            y1 = 0;
            x2 = n == A.nt-1 ? (A.n-1)%A.cols_per_chunk+1 : A.cols_per_chunk;
            y2 = m == A.mt-1 ? (A.m-1)%A.rows_per_chunk+1 : A.rows_per_chunk;

            f77 = &pA[(size_t)A.cols_per_chunk*lda*n + (size_t)A.rows_per_chunk*m];
            bdl = (float*)hw_tile_addr(A, m, n,0,0);

            hw_core_omp_slacpy(hwGeneral, hwNoTrans,
                            y2-y1, x2-x1,
                            &(f77[x1*lda+y1]), lda,
                            &(bdl[x1*A.cols_per_chunk+y1]), ldt,
                            sequence, request);
        }
    }
}