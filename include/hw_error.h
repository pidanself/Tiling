/**
 *
 * @file
 *
 *  hw is a software package provided by:
 *  University of Tennessee, US,
 *  University of Manchester, UK.
 *
 **/
#ifndef hw_ERROR_H
#define hw_ERROR_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#define hw_warning(msg) \
    hw_warning_func_line_file(__func__, __LINE__, __FILE__, msg)

#define hw_error(msg) \
    hw_error_func_line_file(__func__, __LINE__, __FILE__, msg)

#define hw_error_with_code(msg, code) \
    hw_error_func_line_file_code(__func__, __LINE__, __FILE__, msg, \
                                         code)

#define hw_fatal_error(msg) \
    hw_fatal_error_func_line_file(__func__, __LINE__, __FILE__, msg)

/******************************************************************************/
static inline void hw_warning_func_line_file(
    char const *func, int line, const char *file, const char *msg)
{
    fprintf(stderr,
            "hw WARNING at %d of %s() in %s: %s\n",
            line, func, file, msg);
}

/******************************************************************************/
static inline void hw_error_func_line_file(
    char const *func, int line, const char *file, const char *msg)
{
    fprintf(stderr,
            "hw ERROR at %d of %s() in %s: %s\n",
            line, func, file, msg);
}

/******************************************************************************/
static inline void hw_error_func_line_file_code(
    char const *func, int line, const char *file, const char *msg, int code)
{
    fprintf(stderr,
            "hw ERROR at %d of %s() in %s: %s %d\n",
            line, func, file, msg, code);
}

/******************************************************************************/
static inline void hw_fatal_error_func_line_file(
    char const *func, int line, const char *file, const char *msg)
{
    fprintf(stderr,
            "hw FATAL ERROR at %d of %s() in %s: %s\n",
            line, func, file, msg);
    exit(EXIT_FAILURE);
}

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // hw_ERROR_H
