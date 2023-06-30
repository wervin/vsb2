#include "vsb2/graphics/pipelinelayout.h"
#include "vsb2/graphics/device.h"

#include "vsb2/log.h"

enum vsb2_error vsb2_graphics_pipelinelayout_init(struct vsb2_graphics_pipelinelayout *pipelinelayout, struct vsb2_graphics_device *device)
{
    VkPipelineLayoutCreateInfo pipelinelayout_info = {0};
	pipelinelayout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelinelayout_info.flags = 0;
	pipelinelayout_info.setLayoutCount = 0;
	pipelinelayout_info.pSetLayouts = NULL;
	pipelinelayout_info.pushConstantRangeCount = 0;
	pipelinelayout_info.pPushConstantRanges = NULL;

    if (vkCreatePipelineLayout(device->vk_device, &pipelinelayout_info, NULL, &pipelinelayout->vk_pipelinelayout) !=
        VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to create pipeline layout");
        return VSB2_ERROR_VK;
    }

    return VSB2_ERROR_NONE;
}

void vsb2_graphics_pipelinelayout_destroy(struct vsb2_graphics_pipelinelayout *pipelinelayout, struct vsb2_graphics_device *device)
{
    vkDestroyPipelineLayout(device->vk_device, pipelinelayout->vk_pipelinelayout, NULL);
}
