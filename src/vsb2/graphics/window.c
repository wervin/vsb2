#include "vsb2/graphics/window.h"

#include "vsb2/engine.h"
#include "vsb2/log.h"

#include "sake_macro.h"

static void _glfw_resize_cb(GLFWwindow *glfw_window, int width, int height);

enum vsb2_error vsb2_graphics_window_init(struct vsb2_graphics_window *window, struct vsb2_engine_info *info)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window->glfw_window = glfwCreateWindow(
        info->default_width,
        info->default_height,
        info->application_name, NULL, NULL);
    if (!window->glfw_window)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_GLFW, "Failed to create window");
        return VSB2_ERROR_GLFW;
    }

    glfwSetWindowUserPointer(window->glfw_window, window);
    glfwSetFramebufferSizeCallback(window->glfw_window, _glfw_resize_cb);
    window->resized = false;

    return VSB2_ERROR_NONE;
}

VkExtent2D vsb2_graphics_window_get_extent(struct vsb2_graphics_window *window)
{
    int width, height;
    glfwGetFramebufferSize(window->glfw_window, &width, &height);
    return (VkExtent2D){.height = height, .width = width};
}

bool vsb2_graphics_window_should_close(struct vsb2_graphics_window *window)
{
    return glfwWindowShouldClose(window->glfw_window);
}

void vsb2_graphics_window_poll_events(struct vsb2_graphics_window *window)
{
    SAKE_MACRO_UNUSED(window);
    glfwPollEvents();
}

void vsb2_graphics_window_wait_events(struct vsb2_graphics_window *window)
{
    SAKE_MACRO_UNUSED(window);
    glfwWaitEvents();
}

void vsb2_graphics_window_destroy(struct vsb2_graphics_window *window)
{   
    glfwDestroyWindow(window->glfw_window);
    glfwTerminate();
}

static void _glfw_resize_cb(GLFWwindow *glfw_window, int width, int height)
{
    SAKE_MACRO_UNUSED(width);
    SAKE_MACRO_UNUSED(height);
    struct vsb2_graphics_window *window = (struct vsb2_graphics_window *)glfwGetWindowUserPointer(glfw_window);
    window->resized = true;
}
