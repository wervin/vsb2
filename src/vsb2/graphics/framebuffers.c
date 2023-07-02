#include <stdlib.h>

#include "vsb2/graphics/framebuffers.h"
#include "vsb2/graphics/renderpass.h"
#include "vsb2/graphics/swapchain.h"
#include "vsb2/graphics/device.h"
#include "vsb2/graphics/window.h"

#include "vsb2/log.h"

#include "sake_macro.h"

static enum vsb2_error _create_framebuffers(struct vsb2_graphics_framebuffers *framebuffers, 
                                            struct vsb2_graphics_device *device,
                                            struct vsb2_graphics_swapchain *swapchain, 
                                            struct vsb2_graphics_renderpass *renderpass);

enum vsb2_error vsb2_graphics_framebuffers_init(struct vsb2_graphics_framebuffers *framebuffers,
                                       struct vsb2_graphics_device *device,
                                       struct vsb2_graphics_swapchain *swapchain,
                                       struct vsb2_graphics_renderpass *renderpass)
{
    enum vsb2_error status;

    status = _create_framebuffers(framebuffers, device, swapchain, renderpass);
    if (status != VSB2_ERROR_NONE) {
        return status;
    }

    return VSB2_ERROR_NONE; 
}

void vsb2_graphics_framebuffers_destroy(struct vsb2_graphics_framebuffers *framebuffers, struct vsb2_graphics_device *device)
{
    if (framebuffers->vk_framebuffers)
    {
        for (uint32_t i = 0; i < framebuffers->framebuffer_count; i++)
        {
            vkDestroyFramebuffer(device->vk_device, framebuffers->vk_framebuffers[i], NULL);
        }
        free(framebuffers->vk_framebuffers);
    }
}

static enum vsb2_error _create_framebuffers(struct vsb2_graphics_framebuffers *framebuffers, 
                                            struct vsb2_graphics_device *device,
                                            struct vsb2_graphics_swapchain *swapchain, 
                                            struct vsb2_graphics_renderpass *renderpass)
{
    framebuffers->vk_framebuffers = malloc(swapchain->image_count * sizeof(VkFramebuffer));
    framebuffers->framebuffer_count = swapchain->image_count;
    if (framebuffers->vk_framebuffers == NULL) {
        VSB2_LOG_ERROR(VSB2_ERROR_MEMORY, "Failed to allocate memory");
        return VSB2_ERROR_MEMORY;
    }

    for (uint32_t i = 0; i < swapchain->image_count; i++)
    {
        VkFramebufferCreateInfo framebuffer_info = {0};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = renderpass->vk_renderpass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.width = swapchain->vk_extent.width;
        framebuffer_info.height = swapchain->vk_extent.height;
        framebuffer_info.layers = 1;
        framebuffer_info.pAttachments = &swapchain->vk_image_views[i];
        vkCreateFramebuffer(device->vk_device, &framebuffer_info, NULL, &framebuffers->vk_framebuffers[i]);
    }
    return VSB2_ERROR_NONE;
}