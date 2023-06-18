#ifndef VSB2_GRAPHICS_WINDOW_H
#define VSB2_GRAPHICS_WINDOW_H

#include <stdint.h>
#include <stdbool.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vsb2/error.h"

struct vsb2_graphics_window_info
{
    uint32_t default_width;
    uint32_t default_height;
    const char *application_name;
    uint32_t application_version_major;
    uint32_t application_version_minor;
    uint32_t application_version_revision;
};

struct vsb2_graphics_window
{
    uint32_t width;
    uint32_t height;
    GLFWwindow *glfw_window;
};

enum vsb2_error vsb2_graphics_window_init(struct vsb2_graphics_window *window, struct vsb2_graphics_window_info *info);
bool vsb2_graphics_window_should_close(struct vsb2_graphics_window *window);
void vsb2_graphics_window_poll_event(struct vsb2_graphics_window *window);
void vsb2_graphics_window_destroy(struct vsb2_graphics_window *window);

#endif /* VSB2_GRAPHICS_WINDOW_H */