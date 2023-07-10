#include <string.h>
#include <math.h>

#include "sake_macro.h"

#include "vsb2/engine.h"
#include "vsb2/log.h"

static enum vsb2_error _draw(struct vsb2_engine *engine, struct vsb2_engine_info *info);
static enum vsb2_error _graphics_init(struct vsb2_engine *engine, struct vsb2_engine_info *info);
static enum vsb2_error _graphics_recreate_swapchain(struct vsb2_engine *engine, struct vsb2_engine_info *info);

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
    vsb2_graphics_buffers_vertexbuffer_destroy(&engine->vertexbuffer, &engine->device);
    vsb2_graphics_shader_destroy(&engine->vertex_shader, &engine->device);
    vsb2_graphics_shader_destroy(&engine->frag_shader, &engine->device);
    vsb2_graphics_sync_destroy(&engine->sync, &engine->device);
    vsb2_graphics_commandbuffer_destroy(&engine->commandbuffer, &engine->device, &engine->commandpool);
    vsb2_graphics_commandpool_destroy(&engine->commandpool, &engine->device);
    vsb2_graphics_pipeline_destroy(&engine->pipeline, &engine->device);
    vsb2_graphics_pipelinelayout_destroy(&engine->pipelinelayout, &engine->device);
    vsb2_graphics_framebuffers_destroy(&engine->framebuffers, &engine->device);
    vsb2_graphics_renderpass_destroy(&engine->renderpass, &engine->device);
    vsb2_graphics_swapchain_destroy(engine->current_swapchain, &engine->device);
    vsb2_graphics_device_destroy(&engine->device);
    vsb2_graphics_instance_destroy(&engine->instance);
    vsb2_graphics_window_destroy(&engine->window);
}

enum vsb2_error vsb2_engine_run(struct vsb2_engine *engine, struct vsb2_engine_info *info)
{
    enum vsb2_error error = VSB2_ERROR_NONE;
    while (error == VSB2_ERROR_NONE && !vsb2_graphics_window_should_close(&engine->window))
    {
        vsb2_graphics_window_poll_events(&engine->window);
        error = _draw(engine, info);
    }
    return error;
}

static enum vsb2_error _draw(struct vsb2_engine *engine, struct vsb2_engine_info *info)
{
    static uint32_t _frame_count= 0;

    VkResult result = VK_SUCCESS;
    uint32_t current_frame_in_flight = _frame_count % info->max_frames_in_flight;

    if (vkWaitForFences(engine->device.vk_device, 1, &engine->sync.vk_render_fence[current_frame_in_flight], VK_TRUE, 1000000000) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to wait render fence");
        return VSB2_ERROR_VK;
    }

    uint32_t swapchainImageIndex = 0;
    result = vkAcquireNextImageKHR(
        engine->device.vk_device,
        engine->current_swapchain->vk_swapchain,
        1000000000,
        engine->sync.vk_present_semaphore[current_frame_in_flight],
        VK_NULL_HANDLE,
        &swapchainImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_TIMEOUT)
        return _graphics_recreate_swapchain(engine, info);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to acquire next image");
        return VSB2_ERROR_VK;
    }

    if (vkResetFences(engine->device.vk_device, 1, &engine->sync.vk_render_fence[current_frame_in_flight]) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to reset render fence");
        return VSB2_ERROR_VK;
    }

    if (vkResetCommandBuffer(engine->commandbuffer.vk_commandbuffer[current_frame_in_flight], 0) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to reset command buffer");
        return VSB2_ERROR_VK;
    }

    VkCommandBufferBeginInfo commandbufferbegin_info = {0};
    commandbufferbegin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandbufferbegin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    if (vkBeginCommandBuffer(engine->commandbuffer.vk_commandbuffer[current_frame_in_flight], &commandbufferbegin_info) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to reset begin command buffer");
        return VSB2_ERROR_VK;
    }

    VkClearValue clearValue = {0};
    clearValue.color = (VkClearColorValue) {{0.0f, 0.0f, 0.0f, 1.0f}};

    VkRenderPassBeginInfo renderpassbegin_info = {0};
    renderpassbegin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderpassbegin_info.renderPass = engine->renderpass.vk_renderpass;
    renderpassbegin_info.renderArea.offset = (VkOffset2D) {.x = 0, .y = 0};
    renderpassbegin_info.renderArea.extent = engine->current_swapchain->vk_extent;
    renderpassbegin_info.clearValueCount = 1;
    renderpassbegin_info.pClearValues = &clearValue;
    renderpassbegin_info.framebuffer = engine->framebuffers.vk_framebuffers[swapchainImageIndex];
    vkCmdBeginRenderPass(engine->commandbuffer.vk_commandbuffer[current_frame_in_flight], &renderpassbegin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(engine->commandbuffer.vk_commandbuffer[current_frame_in_flight], VK_PIPELINE_BIND_POINT_GRAPHICS, engine->pipeline.vk_pipeline);

    VkViewport viewport = {0};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) engine->current_swapchain->vk_extent.width;
    viewport.height = (float) engine->current_swapchain->vk_extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(engine->commandbuffer.vk_commandbuffer[current_frame_in_flight], 0, 1, &viewport);

    VkRect2D scissor = {0};
    scissor.offset = (VkOffset2D) {.x = 0, .y = 0};
    scissor.extent = engine->current_swapchain->vk_extent;
    vkCmdSetScissor(engine->commandbuffer.vk_commandbuffer[current_frame_in_flight], 0, 1, &scissor);

    VkBuffer vertex_buffers[] = {engine->vertexbuffer.vk_vertexbuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(engine->commandbuffer.vk_commandbuffer[current_frame_in_flight], 0, 1, vertex_buffers, offsets);

    vkCmdDraw(engine->commandbuffer.vk_commandbuffer[current_frame_in_flight], info->vertexbuffer_info.vertex_count, 1, 0, 0);

    vkCmdEndRenderPass(engine->commandbuffer.vk_commandbuffer[current_frame_in_flight]);

    if (vkEndCommandBuffer(engine->commandbuffer.vk_commandbuffer[current_frame_in_flight]) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to reset end command buffer");
        return VSB2_ERROR_VK;
    }

    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submit_info = {0};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &engine->commandbuffer.vk_commandbuffer[current_frame_in_flight];
    submit_info.pWaitDstStageMask = &waitStage;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &engine->sync.vk_present_semaphore[current_frame_in_flight];
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &engine->sync.vk_render_semaphore[current_frame_in_flight];
    if (vkQueueSubmit(engine->device.vk_graphic_queue, 1, &submit_info, engine->sync.vk_render_fence[current_frame_in_flight]) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to submit command buffer to the queue");
        return VSB2_ERROR_VK;
    }

    VkPresentInfoKHR present_info = {0};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.pSwapchains = &engine->current_swapchain->vk_swapchain;
    present_info.swapchainCount = 1;
    present_info.pWaitSemaphores = &engine->sync.vk_render_semaphore[current_frame_in_flight];
    present_info.waitSemaphoreCount = 1;
    present_info.pImageIndices = &swapchainImageIndex;
    result = vkQueuePresentKHR(engine->device.vk_graphic_queue, &present_info);

    _frame_count++;

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || engine->window.resized)
    {   
        engine->window.resized = false;
        return _graphics_recreate_swapchain(engine, info);
    }

    if (result != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to present rendered images");
        return VSB2_ERROR_VK;
    }

    return VSB2_ERROR_NONE;
}

static enum vsb2_error _graphics_init(struct vsb2_engine *engine, struct vsb2_engine_info *info)
{
    enum vsb2_error status;

    status = vsb2_graphics_window_init(&engine->window, info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_instance_init(&engine->instance, &engine->window, info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_device_init(&engine->device, &engine->instance, info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_swapchain_init(&engine->swapchain1, NULL, &engine->window, &engine->instance, &engine->device, info);
    if (status != VSB2_ERROR_NONE)
        return status;

    engine->current_swapchain = &engine->swapchain1;
    engine->old_swapchain = NULL;

    status = vsb2_graphics_renderpass_init(&engine->renderpass, &engine->device, info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_framebuffers_init(&engine->framebuffers, &engine->device, engine->current_swapchain, &engine->renderpass);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_commandpool_init(&engine->commandpool, &engine->device);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_commandbuffer_init(&engine->commandbuffer, &engine->device, &engine->commandpool, info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_sync_init(&engine->sync, &engine->device, info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_shader_init(&engine->vertex_shader, &engine->device, &info->vertex_shader_info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_shader_init(&engine->frag_shader, &engine->device, &info->frag_shader_info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_pipelinelayout_init(&engine->pipelinelayout, &engine->device);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_pipeline_init(
        &engine->pipeline, 
        &engine->device,
        &engine->renderpass,
        &engine->pipelinelayout, 
        &engine->vertex_shader, 
        &engine->frag_shader);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_buffers_vertexbuffer_init(&engine->vertexbuffer, &engine->device, &info->vertexbuffer_info);
    if (status != VSB2_ERROR_NONE)
        return status;
        
    return VSB2_ERROR_NONE;
}


static enum vsb2_error _graphics_recreate_swapchain(struct vsb2_engine *engine, struct vsb2_engine_info *info)
{
    enum vsb2_error status;

    VkExtent2D extent = vsb2_graphics_window_get_extent(&engine->window);
    while(extent.height == 0 || extent.width == 0)
    {
        extent = vsb2_graphics_window_get_extent(&engine->window);
        vsb2_graphics_window_wait_events(&engine->window);
    }

    vsb2_graphics_device_wait_idle(&engine->device);

    vsb2_graphics_framebuffers_destroy(&engine->framebuffers, &engine->device);
    vsb2_graphics_swapchain_destroy(engine->current_swapchain, &engine->device);

    status = vsb2_graphics_swapchain_init(engine->current_swapchain, NULL, &engine->window, &engine->instance, &engine->device, info);
    if (status != VSB2_ERROR_NONE)
        return status;

    status = vsb2_graphics_framebuffers_init(&engine->framebuffers, &engine->device, engine->current_swapchain, &engine->renderpass);
    if (status != VSB2_ERROR_NONE)
        return status;

    return VSB2_ERROR_NONE;
}