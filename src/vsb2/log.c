#include <stdarg.h>
#include <time.h>

#include "vsb2/log.h"

void vsb2_log(FILE * stream, enum vsb2_log_level level, const char *file, uint32_t line, const char *func, const char * format, ...)
{
    time_t t;
    struct tm *tm;
    
#ifndef NDEBUG
    t = time(NULL);
    tm = localtime(&t);
    fprintf(stream, "[%d-%02d-%02d %02d:%02d:%02d] ", 
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    fprintf(stream, "[%s:%u:%s] ", file, line, func);
#endif
    
    fprintf(stream, "[%s] ", vsb2_log_level_str(level));

    va_list args;
    va_start (args, format);
    vfprintf (stream, format, args);
    va_end (args);
}

const char * vsb2_log_level_str(enum vsb2_log_level level)
{
    switch (level)
    {
#define X(def,id,str)   \
    case id:            \
        return str;     \
    
    VSB2_LOG_LEVEL

#undef X

    default:
        return "Unknown level";
    }
}