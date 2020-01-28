#pragma once

#include <vulkan/vulkan.h>

class VulkanInstance;

/**
*	Class to copy a buffer in to another one, or to a image
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

	void Copy(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size);

	void BeginSingleTimeCommand();

	void EndSingleTimeCommand();

	void CopyBufferToImage(VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height);

	void TransitionImageLayout(VkImage& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
};

