#ifndef VSB2_GRAPHICS_INSTANCE_H
#define VSB2_GRAPHICS_INSTANCE_H

#include <stdint.h>

#include <vulkan/vulkan.h>

#include "vsb2/error.h"

struct vsb2_graphics_window;
struct vsb2_engine_info;

#ifndef NDEBUG
struct vsb2_graphics_instance_info
{
  const char** validation_layers;
  const char** debug_extensions;
};
#endif

struct vsb2_graphics_instance
{
  VkInstance vk_instance;
  VkSurfaceKHR vk_surface;
  VkDebugUtilsMessengerEXT vk_debug_messenger;
};

enum vsb2_error vsb2_graphics_instance_init(
    struct vsb2_graphics_instance *instance,
    struct vsb2_graphics_window *window,
    struct vsb2_engine_info *info);

void vsb2_graphics_instance_destroy(struct vsb2_graphics_instance *instance);

#endif /* VSB2_GRAPHICS_INSTANCE_H */