/**
 *
 * @file
 *
 *  hw is a software package provided by:
 *  University of Tennessee, US,
 *  University of Manchester, UK.
 *
 **/
#ifndef hw_BARRIER_H
#define hw_BARRIER_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
typedef struct {
    int count;
    volatile int passed;
} hw_barrier_t;

/******************************************************************************/
void hw_barrier_init(hw_barrier_t *barrier);
void hw_barrier_wait(hw_barrier_t *barrier, int size);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // hw_BARRIER_H
