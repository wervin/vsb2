#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "vsb2/graphics/shader.h"
#include "vsb2/graphics/device.h"

#include "vsb2/log.h"

enum vsb2_error vsb2_graphics_shader_init(struct vsb2_graphics_shader *shader, 
                                          struct vsb2_graphics_device *device, 
                                          struct vsb2_graphics_shader_info *info)
{
    uint32_t size;
    uint8_t *buffer;

    FILE* fd = fopen(info->path, "rb");
    if (!fd) 
    {
        VSB2_LOG_ERROR(VSB2_ERROR_FILE, info->path);
        return VSB2_ERROR_FILE;
    }

    fseek(fd, 0, SEEK_END);
    size = ftell(fd);
    
    buffer = malloc(size * sizeof(uint8_t));
    if (buffer == NULL) {
        VSB2_LOG_ERROR(VSB2_ERROR_MEMORY, "Failed to allocate memory");
        return VSB2_ERROR_MEMORY;
    }

	rewind(fd);
	fread(buffer, 1, size, fd);
    fclose(fd);

    VkShaderModuleCreateInfo create_info = {0};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = size;
    create_info.pCode = (uint32_t *) buffer;
    if (vkCreateShaderModule(device->vk_device, &create_info, NULL, &shader->vk_shader_module) != VK_SUCCESS) 
    {
        free(buffer);
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to create shader module");
        return VSB2_ERROR_VK;
    }

    free(buffer);
    return VSB2_ERROR_NONE;
}

void vsb2_graphics_shader_destroy(struct vsb2_graphics_shader *shader, struct vsb2_graphics_device *device)
{
    vkDestroyShaderModule(device->vk_device, shader->vk_shader_module, NULL);
}
