#pragma once

#include <vulkan/vulkan.h>

class VulkanInstance;

/**
*	Class to copy a buffer in to another one
*	@author Fernando del Molino
*/
class CopyBuffer
{
private:
	VkCommandPool _commandPool = nullptr;
	VkCommandBuffer _commandBuffer = nullptr;

	VulkanInstance* _vk;

	void Initialize();

	bool _init = false;

public:
	CopyBuffer(VulkanInstance* vk);

	~CopyBuffer();

	void Copy(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};

