/**
 *
 * @file
 *
 *  hw is a software package provided by:
 *  University of Tennessee, US,
 *  University of Manchester, UK.
 *
 **/

#include "../include/hw_context.h"
#include "../include/hw_internal.h"
#include "../include/hw_error.h"
//#include "hw_tuning.h"

#include <stdlib.h>
#include <omp.h>

static int max_contexts = 1024;
static int num_contexts = 0;

hw_context_map_t *context_map = NULL;
pthread_mutex_t hw_context_map_lock = PTHREAD_MUTEX_INITIALIZER;

hw_context_t *hw_context_self()
{
    pthread_mutex_lock(&hw_context_map_lock);

    // Find the thread and return its context.
    for (int i = 0; i < max_contexts; i++) {
        if (context_map[i].context != NULL &&
            pthread_equal(context_map[i].thread_id, pthread_self())) {

            pthread_mutex_unlock(&hw_context_map_lock);
            return context_map[i].context;
        }
    }
    pthread_mutex_unlock(&hw_context_map_lock);
    hw_error("context not found");
    return NULL;
}

int hw_init()
{
    pthread_mutex_lock(&hw_context_map_lock);

    // Allocate context map if NULL.
    if (context_map == NULL) {
        context_map =
            (hw_context_map_t*)calloc(max_contexts,
                                          sizeof(hw_context_map_t));
        if (context_map == NULL) {
            pthread_mutex_unlock(&hw_context_map_lock);
            hw_error("calloc() failed");
            return hwErrorOutOfMemory;
        }
    }
    pthread_mutex_unlock(&hw_context_map_lock);

    hw_context_attach();
    return hwSuccess;
}

int hw_context_attach()
{
    pthread_mutex_lock(&hw_context_map_lock);

    // Reallocate context map if out of space.
    if (num_contexts == max_contexts-1) {
        max_contexts *= 2;
        context_map = (hw_context_map_t*) realloc(
            &context_map, max_contexts*sizeof(hw_context_map_t));
        if (context_map == NULL) {
            pthread_mutex_unlock(&hw_context_map_lock);
            hw_error("realloc() failed");
            return hwErrorOutOfMemory;
        }
    }
    // Create the context.
    hw_context_t *context;
    context = (hw_context_t*)malloc(sizeof(hw_context_t));
    if (context == NULL) {
        pthread_mutex_unlock(&hw_context_map_lock);
        hw_error("malloc() failed");
        return hwErrorOutOfMemory;
    }
    // Initialize the context.
    hw_context_init(context);

    // Find and empty slot and insert the context.
    for (int i = 0; i < max_contexts; i++) {
        if (context_map[i].context == NULL) {
            context_map[i].context = context;
            context_map[i].thread_id = pthread_self();
            num_contexts++;
            pthread_mutex_unlock(&hw_context_map_lock);
            return hwSuccess;
        }
    }
    // This should never happen.
    pthread_mutex_unlock(&hw_context_map_lock);
    hw_error("empty slot not found");
    return hwErrorInternal;
}
void hw_context_init(hw_context_t *context)
{
    // Set defaults.
    context->tuning = hwEnabled;
    context->nb = 256;
    context->ib = 64;
    context->inplace_outplace = hwOutplace;
    context->max_threads = omp_get_max_threads();
    context->max_panel_threads = 1;
    context->householder_mode = hwFlatHouseholder;
    
    //先假设此处为不作处理的情况
    //hw_tuning_init(context);
}
