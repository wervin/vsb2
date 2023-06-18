#ifndef VSB2_LOG_H
#define VSB2_LOG_H

#include <stdio.h>
#include <stdint.h>

#include "vsb2/error.h"

#define VSB2_LOG_INFO(...) vsb2_log(stdout, VSB2_LOG_LEVEL_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define VSB2_LOG_ERROR(error, msg) vsb2_log(stderr, VSB2_LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, "[%s] %s\n", vsb2_error_str(error), msg)

#ifdef NDEBUG
#define VSB2_LOG_DEBUG(...)
#else
#define VSB2_LOG_DEBUG(...) vsb2_log(stderr, VSB2_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#endif

#define VSB2_LOG_LEVEL                  \
    X(VSB2_LOG_LEVEL_INFO, 0, "INFO")   \
    X(VSB2_LOG_LEVEL_ERROR, 1, "ERROR") \
    X(VSB2_LOG_LEVEL_DEBUG, 2, "DEBUG")

enum vsb2_log_level
{
#define X(def, id, str) def=id,

    VSB2_LOG_LEVEL

#undef X
};

void vsb2_log(FILE *stream, enum vsb2_log_level level, const char *file, uint32_t line, const char *func, const char *format, ...);
const char * vsb2_log_level_str(enum vsb2_log_level level);

#endif /* VSB2_LOG_H */