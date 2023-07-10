#ifndef VSB2_GRAPHICS_BUFFERS_VERTEXBUFFER_H
#define VSB2_GRAPHICS_BUFFERS_VERTEXBUFFER_H

#include <vulkan/vulkan.h>

#include "vsb2/error.h"

struct vsb2_graphics_device;

struct vsb2_graphics_buffers_vertexbuffer_info
{
    uint32_t vertex_count;
    uint32_t vertex_size;
    VkBufferUsageFlags usage;
    VkMemoryPropertyFlags memory_properties;
    void *data;
};

struct vsb2_graphics_buffers_vertexbuffer
{
    VkBuffer vk_vertexbuffer;
    VkDeviceMemory vk_vertexmemory;
};

enum vsb2_error vsb2_graphics_buffers_vertexbuffer_init(
    struct vsb2_graphics_buffers_vertexbuffer *vertexbuffer, 
    struct vsb2_graphics_device *device,
    struct vsb2_graphics_buffers_vertexbuffer_info *info);

void vsb2_graphics_buffers_vertexbuffer_destroy(struct vsb2_graphics_buffers_vertexbuffer *vertexbuffer, struct vsb2_graphics_device* device);



#endif /* VSB2_GRAPHICS_BUFFERS_VERTEXBUFFER_H */