#ifndef VSB2_GRAPHICS_COMMAND_POOL_H
#define VSB2_GRAPHICS_COMMAND_POOL_H

#include <vulkan/vulkan.h>

#include "vsb2/error.h"

struct vsb2_graphics_device;

struct vsb2_graphics_commandpool
{
  VkCommandPool vk_commandpool;
};

enum vsb2_error vsb2_graphics_commandpool_init(
    struct vsb2_graphics_commandpool *commandpool,
    struct vsb2_graphics_device *device);

void vsb2_graphics_commandpool_destroy(
    struct vsb2_graphics_commandpool *commandpool,
    struct vsb2_graphics_device *device);

#endif /* VSB2_GRAPHICS_COMMAND_POOL_H */