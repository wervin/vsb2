#include "vsb2/graphics/sync.h"
#include "vsb2/graphics/device.h"

#include "vsb2/log.h"

enum vsb2_error vsb2_graphics_sync_init(
    struct vsb2_graphics_sync *sync,
    struct vsb2_graphics_device *device)
{
    VkFenceCreateInfo fence_info = {0};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateFence(device->vk_device, &fence_info, NULL, &sync->vk_render_fence) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to create render fence");
        return VSB2_ERROR_VK;
    }

    VkSemaphoreCreateInfo semaphore_info = {0};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(device->vk_device, &semaphore_info, NULL, &sync->vk_render_semaphore) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to create render semaphore");
        return VSB2_ERROR_VK;
    }
    if (vkCreateSemaphore(device->vk_device, &semaphore_info, NULL, &sync->vk_present_semaphore)  != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to create present semaphore");
        return VSB2_ERROR_VK;
    }

    return VSB2_ERROR_NONE;
}

void vsb2_graphics_sync_destroy(
    struct vsb2_graphics_sync *sync,
    struct vsb2_graphics_device *device)
{
    vkDestroyFence(device->vk_device, sync->vk_render_fence, NULL);
    vkDestroySemaphore(device->vk_device, sync->vk_render_semaphore, NULL);
    vkDestroySemaphore(device->vk_device, sync->vk_present_semaphore, NULL);
}