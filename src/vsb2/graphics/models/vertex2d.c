#include <stddef.h>
#include <stdalign.h>

#include "vsb2/graphics/models/vertex2d.h"

struct vsb2_graphics_models_vertex2d_input vsb2_graphics_models_vertex2d_get_input(void)
{
    struct vsb2_graphics_models_vertex2d_input vertex_input = {
        .attributes = {
            {
                .binding = 0,
                .location = 0,
                .format = VK_FORMAT_R32G32_SFLOAT,
                .offset = offsetof(struct vsb2_graphics_models_vertex2d, position)
            }
        },
        .binding = {
            .binding = 0,
            .stride = sizeof(struct vsb2_graphics_models_vertex2d),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
        }
    };

    return vertex_input;
}