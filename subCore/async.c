/**
 *
 * @file
 *
 *  hw is a software package provided by:
 *  University of Tennessee, US,
 *  University of Manchester, UK.
 *
 **/

#include "../include/hw_async.h"
#include "../include/hw_internal.h"

#include <stdlib.h>

/******************************************************************************/
int hw_request_fail(hw_sequence_t *sequence,
                        hw_request_t *request,
                        int status)
{
    sequence->request = request;
    sequence->status = status;
    request->status = status;
    return status;
}

/******************************************************************************/
int hw_request_init(hw_request_t *request)
{
    request->status = hwSuccess;
    return hwSuccess;
}

/******************************************************************************/
int hw_sequence_init(hw_sequence_t *sequence)
{
    sequence->status = hwSuccess;
    sequence->request = NULL;
    return hwSuccess;
}
