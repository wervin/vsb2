#ifndef VSB2_GRAPHICS_RENDERPASS_H
#define VSB2_GRAPHICS_RENDERPASS_H

#include <vulkan/vulkan.h>

#include "vsb2/error.h"

struct vsb2_graphics_device;
struct vsb2_engine_info;

struct vsb2_graphics_renderpass_info
{
  VkSampleCountFlagBits sample_count;
  VkFormat format;
};

struct vsb2_graphics_renderpass
{
  VkRenderPass vk_renderpass;
};

enum vsb2_error vsb2_graphics_renderpass_init(
    struct vsb2_graphics_renderpass *renderpass,
    struct vsb2_graphics_device *device,
    struct vsb2_engine_info *info);

void vsb2_graphics_renderpass_destroy(
    struct vsb2_graphics_renderpass *renderpass,
    struct vsb2_graphics_device *device);

#endif /* VSB2_GRAPHICS_RENDERPASS_H */