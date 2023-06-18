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
#include "vsb2/graphics/window.h"

struct vsb2_engine_info
{
    struct vsb2_graphics_window_info window_info;
    struct vsb2_graphics_instance_info instance_info;
    struct vsb2_graphics_device_info device_info;
    struct vsb2_graphics_swapchain_info swapchain_info;
    struct vsb2_graphics_renderpass_info renderpass_info;
};

struct vsb2_engine
{   
    struct vsb2_graphics_window window;
    struct vsb2_graphics_instance instance;
    struct vsb2_graphics_device device;
    struct vsb2_graphics_swapchain swapchain;
    struct vsb2_graphics_renderpass renderpass;
    struct vsb2_graphics_framebuffers framebuffers;
    struct vsb2_graphics_commandpool commandpool;
    struct vsb2_graphics_commandbuffer commandbuffer;
};

enum vsb2_error vsb2_engine_init(struct vsb2_engine *engine, struct vsb2_engine_info *info);
void vsb2_engine_cleanup(struct vsb2_engine *engine);
void vsb2_engine_draw(struct vsb2_engine *engine);
void vsb2_engine_run(struct vsb2_engine *engine);

#endif /* VSB2_ENGINE_H */