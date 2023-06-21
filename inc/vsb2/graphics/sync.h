#ifndef VSB2_GRAPHICS_SYNC_H
#define VSB2_GRAPHICS_SYNC_H

#include <stdint.h>

#include <vulkan/vulkan.h>

#include "vsb2/error.h"

struct vsb2_graphics_device;

struct vsb2_graphics_sync
{
    VkFence vk_render_fence;
    VkSemaphore vk_present_semaphore;
    VkSemaphore vk_render_semaphore;
};

enum vsb2_error vsb2_graphics_sync_init(
    struct vsb2_graphics_sync *sync,
    struct vsb2_graphics_device *device);

void vsb2_graphics_sync_destroy(
    struct vsb2_graphics_sync *sync,
    struct vsb2_graphics_device *device);

#endif  /* VSB2_GRAPHICS_SYNC_H */