#ifndef VSB2_GRAPHICS_WINDOW_H
#define VSB2_GRAPHICS_WINDOW_H

#include <stdint.h>
#include <stdbool.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vsb2/error.h"

struct vsb2_engine_info;

struct vsb2_graphics_window
{
    bool resized;
    GLFWwindow *glfw_window;
};

enum vsb2_error vsb2_graphics_window_init(struct vsb2_graphics_window *window, struct vsb2_engine_info *info);
VkExtent2D vsb2_graphics_window_get_extent(struct vsb2_graphics_window *window);
bool vsb2_graphics_window_should_close(struct vsb2_graphics_window *window);
void vsb2_graphics_window_poll_events(struct vsb2_graphics_window *window);
void vsb2_graphics_window_wait_events(struct vsb2_graphics_window *window);
void vsb2_graphics_window_destroy(struct vsb2_graphics_window *window);

#endif /* VSB2_GRAPHICS_WINDOW_H */