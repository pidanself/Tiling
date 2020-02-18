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
        ldt = hw_tile_mmain(A, m);
        for (n = 0; n < A.nt; n++) {
            x1 = n == 0 ? A.j%A.nb : 0;
            y1 = m == 0 ? A.i%A.mb : 0;
            x2 = n == A.nt-1 ? (A.j+A.n-1)%A.nb+1 : A.nb;
            y2 = m == A.mt-1 ? (A.i+A.m-1)%A.mb+1 : A.mb;

            f77 = &pA[(size_t)A.nb*lda*n + (size_t)A.mb*m];
            bdl = (float*)hw_tile_addr(A, m, n);

            hw_core_omp_slacpy(hwGeneral, hwNoTrans,
                            y2-y1, x2-x1,
                            &(f77[x1*lda+y1]), lda,
                            &(bdl[x1*A.nb+y1]), ldt,
                            sequence, request);
        }
    }
}