#ifndef VSB2_GRAPHICS_MODELS_VERTEX2D_H
#define VSB2_GRAPHICS_MODELS_VERTEX2D_H

#include <cglm/cglm.h>

#include <vulkan/vulkan.h>

struct vsb2_graphics_models_vertex2d_input
{
    VkVertexInputBindingDescription binding;
    VkVertexInputAttributeDescription attributes[1];
};

struct vsb2_graphics_models_vertex2d
{
    vec2 position;
};

struct vsb2_graphics_models_vertex2d_input vsb2_graphics_models_vertex2d_get_input(void);

#endif /* VSB2_GRAPHICS_MODELS_VERTEX2D_H */