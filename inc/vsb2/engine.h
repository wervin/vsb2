#ifndef VSB2_ENGINE_H
#define VSB2_ENGINE_H

#include "vsb2/error.h"

#include "vsb2/graphics/commandbuffer.h"
#include "vsb2/graphics/commandpool.h"
#include "vsb2/graphics/device.h"
#include "vsb2/graphics/framebuffers.h"
#include "vsb2/graphics/instance.h"
#include "vsb2/graphics/renderpass.h"
#include "vsb2/graphics/swapchain.h"
#include "vsb2/graphics/sync.h"
#include "vsb2/graphics/window.h"
#include "vsb2/graphics/shader.h"
#include "vsb2/graphics/pipelinelayout.h"
#include "vsb2/graphics/pipeline.h"

#include "vsb2/graphics/buffers/vertexbuffer.h"

#include "vsb2/graphics/models/vertex2d.h"

struct vsb2_engine_info
{
    uint32_t default_width;
    uint32_t default_height;

    const char *application_name;
    uint32_t application_version_major;
    uint32_t application_version_minor;
    uint32_t application_version_revision;

    const char *engine_name;
    uint32_t engine_version_major;
    uint32_t engine_version_minor;
    uint32_t engine_version_revision;

    uint32_t max_frames_in_flight;

#ifndef NDEBUG
    struct vsb2_graphics_instance_info instance_info;
#endif

    struct vsb2_graphics_device_info device_info;
    struct vsb2_graphics_swapchain_info swapchain_info;
    struct vsb2_graphics_renderpass_info renderpass_info;
    struct vsb2_graphics_shader_info vertex_shader_info;
    struct vsb2_graphics_shader_info frag_shader_info;
    struct vsb2_graphics_buffers_vertexbuffer_info vertexbuffer_info;
};

struct vsb2_engine
{       
    struct vsb2_graphics_window window;
    struct vsb2_graphics_instance instance;
    struct vsb2_graphics_device device;
    struct vsb2_graphics_swapchain *current_swapchain;
    struct vsb2_graphics_swapchain *old_swapchain;
    struct vsb2_graphics_swapchain swapchain1;
    struct vsb2_graphics_swapchain swapchain2;
    struct vsb2_graphics_renderpass renderpass;
    struct vsb2_graphics_framebuffers framebuffers;
    struct vsb2_graphics_commandpool commandpool;
    struct vsb2_graphics_commandbuffer commandbuffer;
    struct vsb2_graphics_sync sync;
    struct vsb2_graphics_shader vertex_shader;
    struct vsb2_graphics_shader frag_shader;
    struct vsb2_graphics_pipelinelayout pipelinelayout;
    struct vsb2_graphics_pipeline pipeline;
    struct vsb2_graphics_buffers_vertexbuffer vertexbuffer;
};

enum vsb2_error vsb2_engine_init(struct vsb2_engine *engine, struct vsb2_engine_info *info);
enum vsb2_error vsb2_engine_run(struct vsb2_engine *engine, struct vsb2_engine_info *info);
void vsb2_engine_cleanup(struct vsb2_engine *engine);

#endif /* VSB2_ENGINE_H */