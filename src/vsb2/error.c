#include "vsb2/error.h"

const char* vsb2_error_str(enum vsb2_error err)
{
    switch (err)
    {
#define X(def,id,str)   \
    case id:            \
        return str;     \
    
    VSB2_ERROR

#undef X

    default:
        return "Unknown error";
    }
}