#ifndef VSB2_GRAPHICS_FRAMEBUFFERS_H
#define VSB2_GRAPHICS_FRAMEBUFFERS_H

#include <stdint.h>

#include <vulkan/vulkan.h>

#include "vsb2/error.h"

struct vsb2_graphics_window;
struct vsb2_graphics_instance;
struct vsb2_graphics_device;
struct vsb2_graphics_swapchain;
struct vsb2_graphics_renderpass;

struct vsb2_graphics_framebuffers
{
  VkFramebuffer* vk_framebuffers;
  uint32_t framebuffer_count;
};

enum vsb2_error vsb2_graphics_framebuffers_init(
    struct vsb2_graphics_framebuffers *framebuffers,
    struct vsb2_graphics_window *window,
    struct vsb2_graphics_device *device,
    struct vsb2_graphics_swapchain *swapchain,
    struct vsb2_graphics_renderpass *renderpass);

void vsb2_graphics_framebuffers_destroy(
    struct vsb2_graphics_framebuffers *framebuffers,
    struct vsb2_graphics_device *device);

#endif /* VSB2_GRAPHICS_FRAMEBUFFERS_H */