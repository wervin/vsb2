#include <stdlib.h>

#include "vsb2/graphics/commandpool.h"
#include "vsb2/graphics/device.h"

#include "vsb2/log.h"

#include "sake_macro.h"


enum vsb2_error vsb2_graphics_commandpool_init(struct vsb2_graphics_commandpool *commandpool, struct vsb2_graphics_device *device)
{
    VkCommandPoolCreateInfo commandpool_info = {0};
    commandpool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandpool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandpool_info.queueFamilyIndex = device->vk_graphic_queue_index;
    if (vkCreateCommandPool(device->vk_device, &commandpool_info, NULL, &commandpool->vk_commandpool) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to create command pool");
        return VSB2_ERROR_VK;
    }

    return VSB2_ERROR_NONE;
}

void vsb2_graphics_commandpool_destroy(struct vsb2_graphics_commandpool *commandpool, struct vsb2_graphics_device *device)
{
    vkDestroyCommandPool(device->vk_device, commandpool->vk_commandpool, NULL);
}
