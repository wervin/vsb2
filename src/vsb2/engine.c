#include <string.h>
#include <math.h>

#include "sake_macro.h"

#include "vsb2/engine.h"
#include "vsb2/log.h"

static enum vsb2_error _draw(struct vsb2_engine *engine);
static enum vsb2_error _graphics_init(struct vsb2_engine *engine, struct vsb2_engine_info *info);

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
    vsb2_graphics_device_wait_idle(&engine->device);
    vsb2_graphics_sync_destroy(&engine->sync, &engine->device);
    vsb2_graphics_commandbuffer_destroy(&engine->commandbuffer, &engine->device, &engine->commandpool);
    vsb2_graphics_commandpool_destroy(&engine->commandpool, &engine->device);
    vsb2_graphics_framebuffers_destroy(&engine->framebuffers, &engine->device);
    vsb2_graphics_renderpass_destroy(&engine->renderpass, &engine->device);
    vsb2_graphics_swapchain_destroy(&engine->swapchain, &engine->device);
    vsb2_graphics_device_destroy(&engine->device);
    vsb2_graphics_instance_destroy(&engine->instance);
    vsb2_graphics_window_destroy(&engine->window);
}

enum vsb2_error vsb2_engine_run(struct vsb2_engine *engine)
{
    enum vsb2_error error = VSB2_ERROR_NONE;
    while (error == VSB2_ERROR_NONE && !vsb2_graphics_window_should_close(&engine->window))
    {
        vsb2_graphics_window_poll_event(&engine->window);
        error = _draw(engine);
    }
    return error;
}

static enum vsb2_error _draw(struct vsb2_engine *engine)
{
    static uint32_t _frame = 0;

    // // check if window is minimized and skip drawing
    // if (SDL_GetWindowFlags(_window) & SDL_WINDOW_MINIMIZED)
    //     return;

    // wait until the gpu has finished rendering the last frame. Timeout of 1 second
    if (vkWaitForFences(engine->device.vk_device, 1, &engine->sync.vk_render_fence, true, 1000000000) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to wait render fence");
        return VSB2_ERROR_VK;
    }

    if (vkResetFences(engine->device.vk_device, 1, &engine->sync.vk_render_fence) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to reset render fence");
        return VSB2_ERROR_VK;
    }

    // now that we are sure that the commands finished executing, we can safely reset the command buffer to begin recording again.
    if (vkResetCommandBuffer(engine->commandbuffer.vk_commandbuffer, 0) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to reset command buffer");
        return VSB2_ERROR_VK;
    }

    // request image from the swapchain
    uint32_t swapchainImageIndex = 0;
    if (vkAcquireNextImageKHR(
        engine->device.vk_device, 
        *engine->swapchain.current_swapchain, 
        1000000000, 
        engine->sync.vk_present_semaphore, 
        NULL, 
        &swapchainImageIndex) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to reset acquire next image");
        return VSB2_ERROR_VK;
    }

    // begin the command buffer recording. We will use this command buffer exactly once, so we want to let vulkan know that
    VkCommandBufferBeginInfo commandbufferbegin_info = {0};
    commandbufferbegin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandbufferbegin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    if (vkBeginCommandBuffer(engine->commandbuffer.vk_commandbuffer, &commandbufferbegin_info) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to reset begin command buffer");
        return VSB2_ERROR_VK;
    }

    // make a clear-color from frame number. This will flash with a 120 frame period.
    VkClearValue clearValue = {0};
    float flash = fabs(sin(_frame / 120.f));
    clearValue.color = (VkClearColorValue) {{0.0f, 0.0f, flash, 1.0f}};

    // start the main renderpass.
    // We will use the clear color from above, and the framebuffer of the index the swapchain gave us
    VkRenderPassBeginInfo renderpassbegin_info = {0};
    renderpassbegin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderpassbegin_info.renderPass = engine->renderpass.vk_renderpass;
    renderpassbegin_info.renderArea.offset.x = 0;
    renderpassbegin_info.renderArea.offset.y = 0;
    renderpassbegin_info.renderArea.extent = (VkExtent2D){.height = engine->window.height, .width = engine->window.width};
    renderpassbegin_info.clearValueCount = 1;
    renderpassbegin_info.pClearValues = &clearValue;
    renderpassbegin_info.framebuffer = engine->framebuffers.vk_framebuffers[swapchainImageIndex];
    vkCmdBeginRenderPass(engine->commandbuffer.vk_commandbuffer, &renderpassbegin_info, VK_SUBPASS_CONTENTS_INLINE);

    // once we start adding rendering commands, they will go here

    // finalize the render pass
    vkCmdEndRenderPass(engine->commandbuffer.vk_commandbuffer);

    // finalize the command buffer (we can no longer add commands, but it can now be executed)
    if (vkEndCommandBuffer(engine->commandbuffer.vk_commandbuffer) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to reset end command buffer");
        return VSB2_ERROR_VK;
    }

    // prepare the submission to the queue.
    // we want to wait on the engine->sync.vk_present_semaphore, as that semaphore is signaled when the swapchain is ready
    // we will signal the engine->sync.vk_render_semaphore, to signal that rendering has finished
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submit_info = {0};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.waitSemaphoreCount = 0;
	submit_info.pWaitSemaphores = NULL;
	submit_info.pWaitDstStageMask = NULL;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &engine->commandbuffer.vk_commandbuffer;
	submit_info.signalSemaphoreCount = 0;
	submit_info.pSignalSemaphores = NULL;
    submit_info.pWaitDstStageMask = &waitStage;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &engine->sync.vk_present_semaphore;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &engine->sync.vk_render_semaphore;

    // submit command buffer to the queue and execute it.
    //  engine->sync.vk_render_fence will now block until the graphic commands finish execution
    if (vkQueueSubmit(engine->device.vk_graphic_queue, 1, &submit_info, engine->sync.vk_render_fence) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to submit command buffer to the queue");
        return VSB2_ERROR_VK;
    }

    // prepare present
    //  this will put the image we just rendered to into the visible window.
    //  we want to wait on the engine->sync.vk_render_semaphore for that,
    //  as its necessary that drawing commands have finished before the image is displayed to the user
    VkPresentInfoKHR present_info = {0};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.swapchainCount = 0;
    present_info.pSwapchains = NULL;
    present_info.pWaitSemaphores = NULL;
    present_info.waitSemaphoreCount = 0;
    present_info.pImageIndices = NULL;
    present_info.pSwapchains = engine->swapchain.current_swapchain;
    present_info.swapchainCount = 1;
    present_info.pWaitSemaphores = &engine->sync.vk_render_semaphore;
    present_info.waitSemaphoreCount = 1;
    present_info.pImageIndices = &swapchainImageIndex;
    if (vkQueuePresentKHR(engine->device.vk_graphic_queue, &present_info) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to present rendered images");
        return VSB2_ERROR_VK;
    }

    // increase the number of frames drawn
    _frame++;

    return VSB2_ERROR_NONE;
}

static enum vsb2_error _graphics_init(struct vsb2_engine *engine, struct vsb2_engine_info *info)
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

    status = vsb2_graphics_commandbuffer_init(&engine->commandbuffer, &engine->device, &engine->commandpool);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_sync_init(&engine->sync, &engine->device);
    if (status != VSB2_ERROR_NONE)
        return status;

    return VSB2_ERROR_NONE;
}
