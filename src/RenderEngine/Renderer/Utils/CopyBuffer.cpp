#include "CopyBuffer.h"

#include "../../VulkanInstance.h"
#include <stdexcept>

CopyBuffer::CopyBuffer() {

}



CopyBuffer::~CopyBuffer() {
	if (_init) {
		// clena the commands if created
		vkDestroyCommandPool(VulkanInstance::GetInstance().device, _commandPool, nullptr);
	}
}



void CopyBuffer::Initialize() {
	// gets the queues
	VulkanInstance::QueueFamilyIndices queueFamilyIndices = VulkanInstance::GetInstance().FindQueueFamilies(VulkanInstance::GetInstance().physicalDevice);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	//Pool for graphics commands
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	// create the command pool
	if (vkCreateCommandPool(VulkanInstance::GetInstance().device, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
		throw std::runtime_error("CopyBuffer::Initialize: failed to create command pool!");
	}

	_init = true;
}


void CopyBuffer::BeginSingleTimeCommand() {
	if (!_init) {
		//initialize if not init
		Initialize();
	}

	/*
		Allocate info for the command pool for one command buffer
	*/
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _commandPool;
	allocInfo.commandBufferCount = 1;
	vkAllocateCommandBuffers(VulkanInstance::GetInstance().device, &allocInfo, &_commandBuffer);

	/*
		Begin the command buffer
	*/
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(_commandBuffer, &beginInfo);
}

void CopyBuffer::EndSingleTimeCommand() {
	/*
		End the command buffer
	*/

	vkEndCommandBuffer(_commandBuffer);

	/*
		Submit the command recorded before
	*/
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffer;

	vkQueueSubmit(VulkanInstance::GetInstance().graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	/*
		Wait until the command ends its execution and free the buffer
	*/
	vkQueueWaitIdle(VulkanInstance::GetInstance().graphicsQueue);

	vkFreeCommandBuffers(VulkanInstance::GetInstance().device, _commandPool, 1, &_commandBuffer);
}



void CopyBuffer::Copy(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size) {
	//Start recording commands
	BeginSingleTimeCommand();

	/*
		Copy the buffer
	*/
	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(_commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	EndSingleTimeCommand();
}


void CopyBuffer::CopyBufferToImage(VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height) {

	TransitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	BeginSingleTimeCommand();

	/*
		Copy buffer to image, expecifing width height and prof
	*/
	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(
		_commandBuffer,
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);
	EndSingleTimeCommand();
	TransitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void CopyBuffer::TransitionImageLayout(VkImage& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
	BeginSingleTimeCommand();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;

	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	barrier.srcAccessMask = 0; // TODO
	barrier.dstAccessMask = 0; // TODO

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;
	/*
	 -------------------------------------------------------------------------------------
		UNDERSTAND THIS PART
	 -------------------------------------------------------------------------------------
	*/

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		_commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	EndSingleTimeCommand();
}