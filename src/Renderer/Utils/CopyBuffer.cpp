#include "CopyBuffer.h"

#include "../../VulkanInstance.h"
#include <stdexcept>

CopyBuffer::CopyBuffer(VulkanInstance* vk) : _vk(vk) {

}



CopyBuffer::~CopyBuffer() {
	if (_init) {
		vkFreeCommandBuffers(_vk->device, _commandPool, 1, &_commandBuffer);

		vkDestroyCommandPool(_vk->device, _commandPool, nullptr);
	}
}



void CopyBuffer::Initialize() {
	VulkanInstance::QueueFamilyIndices queueFamilyIndices = _vk->FindQueueFamilies(_vk->physicalDevice);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	//Pool for graphics commands
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(_vk->device, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
		throw std::runtime_error("CopyBuffer::Initialize: failed to create command pool!");
	}

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _commandPool;
	allocInfo.commandBufferCount = 1;

	//Allocate the command buffer
	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(_vk->device, &allocInfo, &commandBuffer);

	_init = true;
}




void CopyBuffer::Copy(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
	if (!_init) {
		Initialize();
	}

	//Start recording commands
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(_commandBuffer, &beginInfo);

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(_commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	//Stop recording commands
	vkEndCommandBuffer(_commandBuffer);

	//Submit the commandbuffer
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffer;

	vkQueueSubmit(_vk->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	// instead of wait for the command to end, maybe wait for a fence to schedule multiple transfers
	vkQueueWaitIdle(_vk->graphicsQueue);
}