#include "hw_async.h"
#include "hw_descriptor.h"
#include "hw_types.h"
#include "hw_barrier.h"
#include "hw_context.h"
#include "hw_internal.h"
void hw_ge2desc(float *pA, int lda,
                      hw_desc_t A,
                      hw_sequence_t *sequence,
                      hw_request_t *request);