#include "vsb2/graphics/renderpass.h"
#include "vsb2/graphics/swapchain.h"
#include "vsb2/graphics/device.h"
#include "vsb2/graphics/instance.h"
#include "vsb2/graphics/window.h"

#include "vsb2/engine.h"
#include "vsb2/log.h"

#include "sake_macro.h"

static enum vsb2_error _create_renderpass(struct vsb2_graphics_renderpass *renderpass, struct vsb2_graphics_device *device, 
    struct vsb2_engine_info *info);

enum vsb2_error vsb2_graphics_renderpass_init(struct vsb2_graphics_renderpass *renderpass, struct vsb2_graphics_device *device,
    struct vsb2_engine_info *info)
{
    enum vsb2_error status;

    status = _create_renderpass(renderpass, device, info);
    if (status != VSB2_ERROR_NONE) {
        return status;
    }

    return VSB2_ERROR_NONE; 
}

void vsb2_graphics_renderpass_destroy(struct vsb2_graphics_renderpass *renderpass, struct vsb2_graphics_device *device)
{
    vkDestroyRenderPass(device->vk_device, renderpass->vk_renderpass, NULL);
}

static enum vsb2_error _create_renderpass(struct vsb2_graphics_renderpass *renderpass, struct vsb2_graphics_device *device, 
    struct vsb2_engine_info *info)
{
    VkAttachmentDescription color_attachment = {0};
    color_attachment.format = info->renderpass_info.format;
    color_attachment.samples = info->renderpass_info.sample_count;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref = {0};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {0};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    VkSubpassDependency dependency = {0};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo render_pass_info = {0};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 1;
    render_pass_info.pAttachments = &color_attachment;
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;
    render_pass_info.dependencyCount = 1;
    render_pass_info.pDependencies = &dependency;
    if (vkCreateRenderPass(device->vk_device, &render_pass_info, NULL, &renderpass->vk_renderpass) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to create render pass");
        return VSB2_ERROR_VK;
    }

    return VSB2_ERROR_NONE;
}