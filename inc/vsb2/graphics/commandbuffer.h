#ifndef VSB2_GRAPHICS_COMMANDBUFFER_H
#define VSB2_GRAPHICS_COMMANDBUFFER_H

#include <vulkan/vulkan.h>

#include "vsb2/error.h"

struct vsb2_graphics_device;
struct vsb2_graphics_commandpool;

struct vsb2_graphics_commandbuffer
{
  VkCommandBuffer vk_commandbuffer;
};

enum vsb2_error vsb2_graphics_commandbuffer_init(
    struct vsb2_graphics_commandbuffer *commandbuffer,
    struct vsb2_graphics_device *device,
    struct vsb2_graphics_commandpool *commandpool);

void vsb2_graphics_commandbuffer_destroy(
    struct vsb2_graphics_commandbuffer *commandbuffer,
    struct vsb2_graphics_device *device,
    struct vsb2_graphics_commandpool *commandpool);

#endif /* VSB2_GRAPHICS_COMMANDBUFFER_H */