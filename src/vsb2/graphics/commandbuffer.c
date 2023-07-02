#include <stdlib.h>

#include "vsb2/graphics/commandbuffer.h"
#include "vsb2/graphics/commandpool.h"
#include "vsb2/graphics/device.h"

#include "vsb2/engine.h"
#include "vsb2/log.h"

#include "sake_macro.h"

enum vsb2_error vsb2_graphics_commandbuffer_init(
    struct vsb2_graphics_commandbuffer *commandbuffer, 
    struct vsb2_graphics_device *device, 
    struct vsb2_graphics_commandpool *commandpool,
    struct vsb2_engine_info *info)
{
    commandbuffer->max_frames_in_flight = info->max_frames_in_flight;
    commandbuffer->vk_commandbuffer = malloc(commandbuffer->max_frames_in_flight * sizeof(VkCommandBuffer));
    if (commandbuffer->vk_commandbuffer == NULL) {
        VSB2_LOG_ERROR(VSB2_ERROR_MEMORY, "Failed to allocate memory");
        return VSB2_ERROR_MEMORY;
    }

    VkCommandBufferAllocateInfo commandbuffer_info = {0};
    commandbuffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandbuffer_info.commandPool = commandpool->vk_commandpool;
    commandbuffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandbuffer_info.commandBufferCount = commandbuffer->max_frames_in_flight;
    if (vkAllocateCommandBuffers(device->vk_device, &commandbuffer_info, commandbuffer->vk_commandbuffer) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to create command buffer");
        return VSB2_ERROR_VK;
    }

    return VSB2_ERROR_NONE;
}

void vsb2_graphics_commandbuffer_destroy(
    struct vsb2_graphics_commandbuffer *commandbuffer, 
    struct vsb2_graphics_device *device,
    struct vsb2_graphics_commandpool *commandpool)
{
    if (commandbuffer->vk_commandbuffer)
    {
        vkFreeCommandBuffers(device->vk_device, commandpool->vk_commandpool, 1, commandbuffer->vk_commandbuffer);
        free(commandbuffer->vk_commandbuffer);
    }
}