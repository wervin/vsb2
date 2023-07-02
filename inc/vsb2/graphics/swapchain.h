#ifndef VSB2_GRAPHICS_SWAPCHAIN_H
#define VSB2_GRAPHICS_SWAPCHAIN_H

#include <stdint.h>

#include <vulkan/vulkan.h>

#include "vsb2/error.h"

struct vsb2_graphics_window;
struct vsb2_graphics_instance;
struct vsb2_graphics_device;
struct vsb2_engine_info;

struct vsb2_graphics_swapchain_info
{
  VkSurfaceFormatKHR vk_surface_format;
  VkPresentModeKHR  vk_present_mode;
  uint32_t image_count;
  uint32_t array_layer_count;
  VkSurfaceTransformFlagBitsKHR transform;
  VkSwapchainCreateFlagBitsKHR create_flags;
  VkImageUsageFlags image_usage_flags;
  VkCompositeAlphaFlagBitsKHR composite_alpha;
  VkBool32 clipped;
};

struct vsb2_graphics_swapchain
{
  VkSwapchainKHR* current_swapchain;
  VkSwapchainKHR vk_swapchain1;
  VkSwapchainKHR vk_swapchain2;
  VkImage* vk_images;
  VkImageView* vk_image_views;
  uint32_t image_count;
};

enum vsb2_error vsb2_graphics_swapchain_init(
    struct vsb2_graphics_swapchain *swapchain,
    struct vsb2_graphics_window *window,
    struct vsb2_graphics_instance *instance,
    struct vsb2_graphics_device *device,
    struct vsb2_engine_info *info);

void vsb2_graphics_swapchain_destroy(
    struct vsb2_graphics_swapchain *swapchain,
    struct vsb2_graphics_device *device);

#endif /* VSB2_GRAPHICS_SWAPCHAIN_H */