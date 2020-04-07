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
	VkFence _wait;

	void Initialize();

	bool _init = false;


	/*
		Begin the command to copy
	*/
	void BeginSingleTimeCommand();

	/*
		End the command to copy
	*/
	void EndSingleTimeCommand();

	/*
		Operations needed to perform the buffer to image copy
	*/
	void TransitionImageLayout(VkImage& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

public:
	CopyBuffer();

	~CopyBuffer();

	/*
		Copy srcBuffer size in to dstBuffer
	*/
	void Copy(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size);

	/*
		Copy data from the buffer to the image
	*/
	void CopyBufferToImage(VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height);
};

