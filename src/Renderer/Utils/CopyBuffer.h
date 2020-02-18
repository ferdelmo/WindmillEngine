#pragma once

#include <vulkan/vulkan.h>

class VulkanInstance;

/**
*	Class to copy a buffer in to another one, or to a image
*	@author Fernando del Molino
*/

/*
	TODO: maybe the class should operate as a class with static functions with only one commandpool created on the first call
*/
class CopyBuffer
{
private:
	VkCommandPool _commandPool = nullptr;
	VkCommandBuffer _commandBuffer = nullptr;

	void Initialize();

	bool _init = false;

public:
	CopyBuffer();

	~CopyBuffer();

	void Copy(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size);

	void BeginSingleTimeCommand();

	void EndSingleTimeCommand();

	void CopyBufferToImage(VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height);

	void TransitionImageLayout(VkImage& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
};

