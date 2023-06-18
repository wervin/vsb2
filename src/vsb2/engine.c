#include <string.h>

#include "sake_macro.h"

#include "vsb2/engine.h"
#include "vsb2/log.h"

enum vsb2_error _graphics_init(struct vsb2_engine *engine, struct vsb2_engine_info *info);

enum vsb2_error vsb2_engine_init(struct vsb2_engine *engine, struct vsb2_engine_info *info)
{
    enum vsb2_error status;

    status = _graphics_init(engine, info);
    if (status != VSB2_ERROR_NONE)
        return status;

    return status;
}

void vsb2_engine_cleanup(struct vsb2_engine *engine)
{
    vsb2_graphics_commandbuffer_destroy(&engine->commandbuffer, &engine->device, &engine->commandpool);
    vsb2_graphics_commandpool_destroy(&engine->commandpool, &engine->device);
    vsb2_graphics_framebuffers_destroy(&engine->framebuffers, &engine->device);
    vsb2_graphics_renderpass_destroy(&engine->renderpass, &engine->device);
    vsb2_graphics_swapchain_destroy(&engine->swapchain, &engine->device);
    vsb2_graphics_device_destroy(&engine->device);
    vsb2_graphics_instance_destroy(&engine->instance);
    vsb2_graphics_window_destroy(&engine->window);
}

void vsb2_engine_draw(struct vsb2_engine *engine)
{
    SAKE_MACRO_UNUSED(engine);
}

void vsb2_engine_run(struct vsb2_engine *engine)
{
    while (!vsb2_graphics_window_should_close(&engine->window))
    {
        vsb2_graphics_window_poll_event(&engine->window);
        vsb2_engine_draw(engine);
    }
}

enum vsb2_error _graphics_init(struct vsb2_engine *engine, struct vsb2_engine_info *info)
{
    enum vsb2_error status;

    status = vsb2_graphics_window_init(&engine->window, &info->window_info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_instance_init(&engine->instance, &engine->window, &info->instance_info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_device_init(&engine->device, &engine->instance, &info->device_info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_swapchain_init(&engine->swapchain, &engine->window, &engine->instance, &engine->device, &info->swapchain_info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_renderpass_init(&engine->renderpass, &engine->device, &info->renderpass_info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_framebuffers_init(&engine->framebuffers, &engine->window, &engine->device, &engine->swapchain, &engine->renderpass);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_commandpool_init(&engine->commandpool, &engine->device);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_commandbuffer_init(&engine->commandbuffer, &engine->device, &engine->commandpool);
    if (status != VSB2_ERROR_NONE)
        return status;

    return VSB2_ERROR_NONE;
}

// void _init_sync_structures()
// {
//     // create syncronization structures
//     // one fence to control when the gpu has finished rendering the frame,
//     // and 2 semaphores to syncronize rendering with swapchain
//     // we want the fence to start signalled so we can wait on it on the first frame
//     VkFenceCreateInfo fenceCreateInfo = vkinit::fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT);

//     VK_CHECK(vkCreateFence(_device, &fenceCreateInfo, NULL, &_renderFence));

//     VkSemaphoreCreateInfo semaphoreCreateInfo = vkinit::semaphore_create_info();

//     VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, NULL, &_presentSemaphore));
//     VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, NULL, &_renderSemaphore));
// }
