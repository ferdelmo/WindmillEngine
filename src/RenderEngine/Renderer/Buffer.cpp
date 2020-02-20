#include "Buffer.h"


Buffer::Buffer() {
	_init = false;
}



Buffer::~Buffer() {

	vkDestroyBuffer(VulkanInstance::GetInstance().device, _buffer, nullptr);
	vkFreeMemory(VulkanInstance::GetInstance().device, _bufferMemory, nullptr);
}


VkBuffer& Buffer::GetBuffer() {
	return _buffer;
}



VkDeviceMemory& Buffer::GetBufferMemory() {
	return _bufferMemory;
}

/*
	Finde the memory type use it for the vertex buffer
*/
uint32_t Buffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(VulkanInstance::GetInstance().physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) &&
			(memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("Buffer::FindMemoryType: failed to find suitable memory type!");
}