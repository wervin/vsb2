#ifndef VSB2_GRAPHICS_PIPELINE_H
#define VSB2_GRAPHICS_PIPELINE_H

#include <vulkan/vulkan.h>

#include "vsb2/error.h"

struct vsb2_graphics_window;
struct vsb2_graphics_device;
struct vsb2_graphics_shader;
struct vsb2_graphics_pipelinelayout;
struct vsb2_graphics_renderpass;

struct vsb2_graphics_pipeline
{
    VkPipeline vk_pipeline;
};

enum vsb2_error vsb2_graphics_pipeline_init(struct vsb2_graphics_pipeline *pipeline,
                                            struct vsb2_graphics_device *device,
                                            struct vsb2_graphics_window *window,
                                            struct vsb2_graphics_renderpass *renderpass,
                                            struct vsb2_graphics_pipelinelayout *pipelinelayout,
                                            struct vsb2_graphics_shader *vertex_shader,
                                            struct vsb2_graphics_shader *frag_shader);
void vsb2_graphics_pipeline_destroy(struct vsb2_graphics_pipeline *pipeline, struct vsb2_graphics_device *device);

#endif /* VSB2_GRAPHICS_PIPELINE_H */