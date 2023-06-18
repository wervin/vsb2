#include "vsb2/engine.h"

int main(void)
{
    struct vsb2_engine_info info = {
        .window_info = {
            .default_width = 800,
            .default_height = 600,
            .application_name = "First Application",
            .application_version_major = 1,
            .application_version_minor = 0,
            .application_version_revision = 0
        },
        .instance_info = {
            .application_name = "First Application",
            .application_version_major = 1,
            .application_version_minor = 0,
            .application_version_revision = 0,
            .engine_name = "VSB2",
            .engine_version_major = 1,
            .engine_version_minor = 0,
            .engine_version_revision = 0,
            .validation_layers = (const char *[]) {"VK_LAYER_KHRONOS_validation"},
            .debug_extensions = (const char *[]) {"VK_EXT_debug_utils"}
        },
        .device_info = {
            .features_requested = {
                .samplerAnisotropy = VK_TRUE
            },
            .device_extensions = (const char *[]) {"VK_KHR_swapchain"},
            .validation_layers = (const char *[]) {"VK_LAYER_KHRONOS_validation"}
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
        }
    };

    struct vsb2_engine engine = {0};

    vsb2_engine_init(&engine, &info);
    vsb2_engine_run(&engine);
    vsb2_engine_cleanup(&engine);

    return 0;
}