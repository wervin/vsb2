#ifndef VSB2_GRAPHICS_SHADER_H
#define VSB2_GRAPHICS_SHADER_H

#include <vulkan/vulkan.h>

#include "vsb2/error.h"

struct vsb2_graphics_device;

struct vsb2_graphics_shader_info
{
    const char *path;
};

struct vsb2_graphics_shader
{
    VkShaderModule vk_shader_module;
};

enum vsb2_error vsb2_graphics_shader_init(struct vsb2_graphics_shader *shader, struct vsb2_graphics_device *device, struct vsb2_graphics_shader_info *info);
void vsb2_graphics_shader_destroy(struct vsb2_graphics_shader *shader, struct vsb2_graphics_device *device);

#endif /* VSB2_GRAPHICS_SHADER_H */