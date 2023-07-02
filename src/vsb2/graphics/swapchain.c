#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "vsb2/graphics/swapchain.h"
#include "vsb2/graphics/device.h"
#include "vsb2/graphics/instance.h"
#include "vsb2/graphics/window.h"

#include "vsb2/engine.h"
#include "vsb2/log.h"

#include "sake_macro.h"

static enum vsb2_error _create_swapchain(
    struct vsb2_graphics_swapchain *swapchain,
    struct vsb2_graphics_swapchain *old_swapchain,
    struct vsb2_graphics_window *window, struct vsb2_graphics_instance *instance, 
    struct vsb2_graphics_device *device, struct vsb2_engine_info *info);

static bool _support_desired_surface_format(struct vsb2_graphics_instance *instance,
                                            struct vsb2_graphics_device *device,
                                            struct vsb2_engine_info *info);
static bool _support_desired_present_mode(struct vsb2_graphics_instance *instance,
                                          struct vsb2_graphics_device *device, 
                                          struct vsb2_engine_info *info);
static bool _support_desired_image_count(struct vsb2_graphics_instance *instance,
                                         struct vsb2_graphics_device *device,
                                         struct vsb2_engine_info *info);
static bool _support_desired_array_layer_count(struct vsb2_graphics_instance *instance,
                                               struct vsb2_graphics_device *device,
                                               struct vsb2_engine_info *info);
static VkExtent2D _get_current_extent(struct vsb2_graphics_instance *instance,
                                      struct vsb2_graphics_device *device,
                                      struct vsb2_graphics_window *window);

enum vsb2_error vsb2_graphics_swapchain_init(
    struct vsb2_graphics_swapchain *swapchain,
    struct vsb2_graphics_swapchain *old_swapchain,
    struct vsb2_graphics_window *window, struct vsb2_graphics_instance *instance, 
    struct vsb2_graphics_device *device, struct vsb2_engine_info *info)
{
    enum vsb2_error status;

    status = _create_swapchain(swapchain, old_swapchain, window, instance, device, info);
    if (status != VSB2_ERROR_NONE) {
        return status;
    }

    return VSB2_ERROR_NONE; 
}

void vsb2_graphics_swapchain_destroy(struct vsb2_graphics_swapchain *swapchain, struct vsb2_graphics_device *device)
{
    if (swapchain->vk_image_views)
    {
        for (uint32_t i = 0; i < swapchain->image_count; i++)
            vkDestroyImageView(device->vk_device, swapchain->vk_image_views[i], NULL);
        free(swapchain->vk_image_views);
    }
    
    if (swapchain->vk_images)
        free(swapchain->vk_images);

    vkDestroySwapchainKHR(device->vk_device, swapchain->vk_swapchain, NULL);
}

static enum vsb2_error _create_swapchain(
    struct vsb2_graphics_swapchain *swapchain, 
    struct vsb2_graphics_swapchain *old_swapchain,
    struct vsb2_graphics_window *window, struct vsb2_graphics_instance *instance, 
    struct vsb2_graphics_device *device, struct vsb2_engine_info *info)
{
    if (!_support_desired_surface_format(instance, device, info))
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "No swapchain support for desired surface format");
        return VSB2_ERROR_VK;
    }

    if (!_support_desired_present_mode(instance, device, info))
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "No swapchain support for desired present mode");
        return VSB2_ERROR_VK;
    }

    if (!_support_desired_image_count(instance, device, info))
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "No swapchain support for desired image count");
        return VSB2_ERROR_VK;
    }

    if (!_support_desired_array_layer_count(instance, device, info))
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "No swapchain support for desired array layer count");
        return VSB2_ERROR_VK;
    }

    swapchain->vk_extent = _get_current_extent(instance, device, window);

    VkSwapchainCreateInfoKHR swapchain_create_info = {0};
    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.flags = info->swapchain_info.create_flags;
    swapchain_create_info.surface = instance->vk_surface;
    swapchain_create_info.minImageCount = info->swapchain_info.image_count;
    swapchain_create_info.imageFormat = info->swapchain_info.vk_surface_format.format;
    swapchain_create_info.imageColorSpace = info->swapchain_info.vk_surface_format.colorSpace;
    swapchain_create_info.imageExtent = swapchain->vk_extent;
    swapchain_create_info.imageArrayLayers = info->swapchain_info.array_layer_count;
    swapchain_create_info.imageUsage = info->swapchain_info.image_usage_flags;

    uint32_t queue_family_indices[] = {device->vk_graphic_queue_index, device->vk_present_queue_index};
    if (device->vk_graphic_queue_index != device->vk_present_queue_index) {
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_create_info.queueFamilyIndexCount = 2;
        swapchain_create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->vk_physical_device, 
                                              instance->vk_surface,
                                              &capabilities);

    swapchain_create_info.preTransform = info->swapchain_info.transform ? info->swapchain_info.transform : capabilities.currentTransform;
    swapchain_create_info.compositeAlpha = info->swapchain_info.composite_alpha;
    swapchain_create_info.presentMode = info->swapchain_info.vk_present_mode;
    swapchain_create_info.clipped = info->swapchain_info.clipped;
    swapchain_create_info.oldSwapchain = old_swapchain ? old_swapchain->vk_swapchain : NULL;
    if (vkCreateSwapchainKHR(device->vk_device, &swapchain_create_info, NULL, &swapchain->vk_swapchain) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to create swap chain");
        return VSB2_ERROR_VK;
    }

    vkGetSwapchainImagesKHR(device->vk_device, swapchain->vk_swapchain, &swapchain->image_count, NULL);

    swapchain->vk_images = malloc(swapchain->image_count * sizeof(VkImage));
    if (swapchain->vk_images == NULL) {
        VSB2_LOG_ERROR(VSB2_ERROR_MEMORY, "Failed to allocate memory");
        return VSB2_ERROR_MEMORY;
    }

    vkGetSwapchainImagesKHR(device->vk_device, swapchain->vk_swapchain, &swapchain->image_count, swapchain->vk_images);

    swapchain->vk_image_views = malloc(swapchain->image_count * sizeof(VkImageView));
    if (swapchain->vk_image_views == NULL) {
        VSB2_LOG_ERROR(VSB2_ERROR_MEMORY, "Failed to allocate memory");
        return VSB2_ERROR_MEMORY;
    }

    for (uint32_t i = 0; i < swapchain->image_count; i++)
    {
        VkImageViewCreateInfo imageview_createinfo = {0};
        imageview_createinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageview_createinfo.image = swapchain->vk_images[i];
        imageview_createinfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageview_createinfo.format = info->swapchain_info.vk_surface_format.format;
        imageview_createinfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageview_createinfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageview_createinfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageview_createinfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageview_createinfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageview_createinfo.subresourceRange.baseMipLevel = 0;
        imageview_createinfo.subresourceRange.levelCount = 1;
        imageview_createinfo.subresourceRange.baseArrayLayer = 0;
        imageview_createinfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(device->vk_device, &imageview_createinfo, NULL, &swapchain->vk_image_views[i]) != VK_SUCCESS)
        {
            VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to create image view");
            return VSB2_ERROR_VK;
        }
    }

    return VSB2_ERROR_NONE;
}

static bool _support_desired_surface_format(struct vsb2_graphics_instance *instance,
                                            struct vsb2_graphics_device *device,
                                            struct vsb2_engine_info *info)
{
    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device->vk_physical_device, 
                                         instance->vk_surface, 
                                         &format_count, 
                                         NULL);

    VkSurfaceFormatKHR formats[format_count];
    vkGetPhysicalDeviceSurfaceFormatsKHR(device->vk_physical_device, 
                                         instance->vk_surface,
                                         &format_count, 
                                         formats);

    for (uint32_t i = 0; i < format_count; i++)
    {
        if ((formats[i].colorSpace == info->swapchain_info.vk_surface_format.colorSpace) &&
            (formats[i].format == info->swapchain_info.vk_surface_format.format))
            return true;
    }
    
    return false;
}

static bool _support_desired_present_mode(struct vsb2_graphics_instance *instance,
                                          struct vsb2_graphics_device *device, 
                                          struct vsb2_engine_info *info)
{
    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device->vk_physical_device, 
                                             instance->vk_surface, 
                                             &present_mode_count, 
                                             NULL);
    
    VkPresentModeKHR present_modes[present_mode_count];
    vkGetPhysicalDeviceSurfacePresentModesKHR(device->vk_physical_device, 
                                              instance->vk_surface,
                                              &present_mode_count,
                                              present_modes);
    
    for (uint32_t i = 0; i < present_mode_count; i++)
    {
        if (present_modes[i] == info->swapchain_info.vk_present_mode)
            return true;
    }
    
    return false;
}

static bool _support_desired_image_count(struct vsb2_graphics_instance *instance,
                                         struct vsb2_graphics_device *device,
                                         struct vsb2_engine_info *info)
{
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->vk_physical_device, 
                                              instance->vk_surface,
                                              &capabilities);
    bool supported_min_image_count = info->swapchain_info.image_count >= capabilities.minImageCount;
    bool supported_max_image_count = !capabilities.maxImageCount || info->swapchain_info.image_count <= capabilities.maxImageCount;
    return supported_max_image_count && supported_min_image_count;
}

static VkExtent2D _get_current_extent(struct vsb2_graphics_instance *instance,
                                      struct vsb2_graphics_device *device,
                                      struct vsb2_graphics_window *window)
{
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->vk_physical_device, 
                                              instance->vk_surface,
                                              &capabilities);

    if (capabilities.currentExtent.width != UINT_MAX) 
    {
        return capabilities.currentExtent;
    } 
    else 
    {
        VkExtent2D extent = vsb2_graphics_window_get_extent(window);
        extent.width = fmax(capabilities.minImageExtent.width,
                                  fmin(capabilities.maxImageExtent.width, extent.width));
        extent.height = fmax(capabilities.minImageExtent.height,
                                   fmin(capabilities.maxImageExtent.height, extent.height));
        return extent;
    }
}

static bool _support_desired_array_layer_count(struct vsb2_graphics_instance *instance,
                                               struct vsb2_graphics_device *device,
                                               struct vsb2_engine_info *info)
{
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->vk_physical_device, 
                                              instance->vk_surface,
                                              &capabilities);
    return info->swapchain_info.array_layer_count <= capabilities.maxImageArrayLayers;
}