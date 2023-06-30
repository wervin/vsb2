#ifndef VSB2_GRAPHICS_PIPELINELAYOUT_H
#define VSB2_GRAPHICS_PIPELINELAYOUT_H

#include <vulkan/vulkan.h>

#include "vsb2/error.h"

struct vsb2_graphics_device;

struct vsb2_graphics_pipelinelayout
{
    VkPipelineLayout vk_pipelinelayout;
};

enum vsb2_error vsb2_graphics_pipelinelayout_init(struct vsb2_graphics_pipelinelayout *pipelinelayout, struct vsb2_graphics_device *device);
void vsb2_graphics_pipelinelayout_destroy(struct vsb2_graphics_pipelinelayout *pipelinelayout, struct vsb2_graphics_device *device);

#endif /* VSB2_GRAPHICS_PIPELINELAYOUT_H */