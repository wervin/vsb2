#ifndef VSB2_ERROR_H
#define VSB2_ERROR_H

#define VSB2_ERROR                                                           \
    X(VSB2_ERROR_NONE,           0,  "No error")                             \
    X(VSB2_ERROR_VALUE,         -1,  "Value error")                          \
    X(VSB2_ERROR_FILE,          -2,  "File error")                           \
    X(VSB2_ERROR_MEMORY,        -3,  "Memory error")                         \
    X(VSB2_ERROR_VK,            -4,  "Vulkan error")                         \
    X(VSB2_ERROR_GLFW,          -5,  "GLFW error")                           \

enum vsb2_error 
{
#define X(def, id, str) def=id,

    VSB2_ERROR

#undef X
};

const char * vsb2_error_str(enum vsb2_error err);

#endif /* VSB2_ERROR_H */