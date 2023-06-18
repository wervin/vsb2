#ifndef VSB2_GRAPHICS_DEVICE_H
#define VSB2_GRAPHICS_DEVICE_H

#include <stdint.h>

#include <vulkan/vulkan.h>

#include "vsb2/error.h"

struct vsb2_graphics_instance;

struct vsb2_graphics_device_info
{
  VkPhysicalDeviceFeatures features_requested;
  const char** device_extensions;
#ifndef NDEBUG
  const char** validation_layers;
#endif
};

struct vsb2_graphics_device
{ 
  VkPhysicalDevice vk_physical_device;
  VkDevice vk_device;
  VkPhysicalDeviceProperties vk_physical_device_properties;
  VkQueue vk_graphic_queue;
  uint32_t vk_graphic_queue_index;
  VkQueue vk_present_queue;
  uint32_t vk_present_queue_index;
};

enum vsb2_error vsb2_graphics_device_init(
    struct vsb2_graphics_device *device,
    struct vsb2_graphics_instance *instance,
    struct vsb2_graphics_device_info *info);

void vsb2_graphics_device_destroy(struct vsb2_graphics_device *device);

#endif /* VSB2_GRAPHICS_DEVICE_H */