#include "vsb2/engine.h"

int main(void)
{
    enum vsb2_error status = VSB2_ERROR_NONE;

    struct vsb2_engine_info info = {
        .default_width = 800,
        .default_height = 600,

        .application_name = "First Application",
        .application_version_major = 1,
        .application_version_minor = 0,
        .application_version_revision = 0,

        .engine_name = "VSB2",
        .engine_version_major = 1,
        .engine_version_minor = 0,
        .engine_version_revision = 0,

        .max_frames_in_flight = 2,

#ifndef NDEBUG
        .instance_info = {
            .validation_layers = (const char *[]) {"VK_LAYER_KHRONOS_validation"},
            .debug_extensions = (const char *[]) {"VK_EXT_debug_utils"},
        },
#endif
        .device_info = {
            .features_requested = {
                .samplerAnisotropy = VK_TRUE,
            },
            .device_extensions = (const char *[]) {"VK_KHR_swapchain"},
        },
        .swapchain_info = {
            .array_layer_count = 1,
            .clipped = VK_TRUE,
            .composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .create_flags = 0,
            .image_count = 3,
            .image_usage_flags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .transform = 0,
            .vk_present_mode = VK_PRESENT_MODE_FIFO_KHR,
            .vk_surface_format = (VkSurfaceFormatKHR) {.format=VK_FORMAT_B8G8R8A8_SRGB, .colorSpace=VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}
        },
        .renderpass_info = {
            .format = VK_FORMAT_B8G8R8A8_SRGB,
            .sample_count = VK_SAMPLE_COUNT_1_BIT
        },
        .vertex_shader_info = {
            .path = "triangle.vert.spv"
        },
        .frag_shader_info = {
            .path = "triangle.frag.spv"
        }
    };

    struct vsb2_engine engine = {0};    

    status = vsb2_engine_init(&engine, &info);
    if (status != VSB2_ERROR_NONE)
        goto cleanup;

    status = vsb2_engine_run(&engine, &info);
    if (status != VSB2_ERROR_NONE)
        goto cleanup;

cleanup:
    vsb2_engine_cleanup(&engine);
    return status;
}