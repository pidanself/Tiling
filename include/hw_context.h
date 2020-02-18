/**
 *
 * @file
 *
 *  hw is a software package provided by:
 *  University of Tennessee, US,
 *  University of Manchester, UK.
 *
 **/
#ifndef hw_CONTEXT_H
#define hw_CONTEXT_H

#include "hw_types.h"
#include "hw_barrier.h"

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
typedef struct {
    int tuning;                     ///< hwEnabled or hwDisabled
    int nb;                         ///< hwNb
    int ib;                         ///< hwIb
    hw_enum_t inplace_outplace; ///< hwInplaceOutplace
    int max_threads;                ///< the value of OMP_NUM_THREADS
    int max_panel_threads;          ///< max threads for panel factorization
    hw_barrier_t barrier;       ///< thread barrier for multithreaded tasks
    hw_enum_t householder_mode; ///< hwHouseholderMode
    void *L;                        ///< Lua state pointer; unusued when Lua is missing
} hw_context_t;

typedef struct {
    pthread_t thread_id;       ///< thread id
    hw_context_t *context; ///< pointer to associated context
} hw_context_map_t;

/******************************************************************************/
hw_context_t *hw_context_self();
int hw_context_attach();
int hw_init();
void hw_context_init(hw_context_t *context);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // hw_CONTEXT_H
