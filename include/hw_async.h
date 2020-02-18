/**
 *
 * @file
 *
 *  hw is a software package provided by:
 *  University of Tennessee, US,
 *  University of Manchester, UK.
 *
 **/
#ifndef hw_ASYNC_H
#define hw_ASYNC_H

#include "hw_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
typedef struct {
    hw_enum_t status; ///< error code
} hw_request_t;

typedef struct {
    hw_enum_t status;      ///< error code
    hw_request_t *request; ///< failed request
} hw_sequence_t;

/******************************************************************************/
int hw_request_fail(hw_sequence_t *sequence,
                        hw_request_t *request,
                        int status);

int hw_request_init(hw_request_t *request);

int hw_sequence_init(hw_sequence_t *sequence);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // hw_ASYNC_H
