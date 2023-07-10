#include <stdlib.h>
#include <string.h>

#include "vsb2/graphics/buffers/vertexbuffer.h"
#include "vsb2/graphics/device.h"

#include "vsb2/log.h"

enum vsb2_error vsb2_graphics_buffers_vertexbuffer_init(
    struct vsb2_graphics_buffers_vertexbuffer *vertexbuffer, 
    struct vsb2_graphics_device *device,
    struct vsb2_graphics_buffers_vertexbuffer_info *info)
{
    VkBufferCreateInfo buffer_info = {0};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = info->vertex_count * info->vertex_size;
    buffer_info.usage = info->usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateBuffer(device->vk_device, &buffer_info, NULL, &vertexbuffer->vk_vertexbuffer) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to create vertex buffer");
        return VSB2_ERROR_VK;
    }

    VkMemoryRequirements mem_requirements = {0};
    vkGetBufferMemoryRequirements(device->vk_device, vertexbuffer->vk_vertexbuffer, &mem_requirements);

    VkPhysicalDeviceMemoryProperties mem_poperties = {0};
    vkGetPhysicalDeviceMemoryProperties(device->vk_physical_device, &mem_poperties);

    uint32_t memory_type_index = 0;
    while (
        (memory_type_index < mem_poperties.memoryTypeCount) &&
        !((mem_requirements.memoryTypeBits & (1 << memory_type_index)) &&
          ((mem_poperties.memoryTypes[memory_type_index].propertyFlags & info->memory_properties) == info->memory_properties)))
    {
        memory_type_index++;
    }

    VkMemoryAllocateInfo alloc_info = {0};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_requirements.size;
    alloc_info.memoryTypeIndex = memory_type_index;
    if (vkAllocateMemory(device->vk_device, &alloc_info, NULL, &vertexbuffer->vk_vertexmemory) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to allocate vertex buffer memory");
        return VSB2_ERROR_VK;
    }

    vkBindBufferMemory(device->vk_device, vertexbuffer->vk_vertexbuffer, vertexbuffer->vk_vertexmemory, 0);

    void* data;
    vkMapMemory(device->vk_device, vertexbuffer->vk_vertexmemory, 0, buffer_info.size, 0, &data);
    memcpy(data, info->data, buffer_info.size);
    vkUnmapMemory(device->vk_device, vertexbuffer->vk_vertexmemory);

    return VSB2_ERROR_NONE;
}

void vsb2_graphics_buffers_vertexbuffer_destroy(struct vsb2_graphics_buffers_vertexbuffer *vertexbuffer, struct vsb2_graphics_device* device)
{
    vkDestroyBuffer(device->vk_device, vertexbuffer->vk_vertexbuffer, NULL);
    vkFreeMemory(device->vk_device, vertexbuffer->vk_vertexmemory, NULL);
}


// enum vsb_error vsb_device_create_buffer(struct y)
// {
//     VkBufferCreateInfo buffer_info = {0};
//     buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//     buffer_info.size = size;
//     buffer_info.usage = usage;
//     buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

//     if (vkCreateBuffer(device->vk_device, &buffer_info, NULL, buffer) != VK_SUCCESS)
//     {
//         VSB_ERROR(VSB_ERROR_VK, "Failed to create vertex buffer");
//         return VSB_ERROR_VK;
//     }

//     VkMemoryRequirements mem_requirements;
//     vkGetBufferMemoryRequirements(device->vk_device, *buffer, &mem_requirements);

//     VkMemoryAllocateInfo alloc_info = {0};
//     alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//     alloc_info.allocationSize = mem_requirements.size;
//     if (!vsb_device_get_memory_type_index(device, mem_requirements.memoryTypeBits, properties, &alloc_info.memoryTypeIndex))
//     {
//         VSB_ERROR(VSB_ERROR_VK, "Failed to allocate vertex buffer memory");
//         return VSB_ERROR_VK;
//     }

//     if (vkAllocateMemory(device->vk_device, &alloc_info, NULL, buffer_memory) != VK_SUCCESS)
//     {
//         VSB_ERROR(VSB_ERROR_VK, "Failed to allocate vertex buffer memory");
//         return VSB_ERROR_VK;
//     }

//     vkBindBufferMemory(device->vk_device, *buffer, *buffer_memory, 0);

//     return VSB_ERROR_NONE;
// }