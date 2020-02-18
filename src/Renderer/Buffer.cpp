#include "Buffer.h"


Buffer::Buffer() {
	_init = false;
}



Buffer::~Buffer() {

	vkDestroyBuffer(VulkanInstance::GetInstance().device, _buffer, nullptr);
	vkFreeMemory(VulkanInstance::GetInstance().device, _bufferMemory, nullptr);
}


/*
template <typename T>
void Buffer::Initialize(const std::vector<T>& list,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties) {

	if (list.size() == 0) {
		throw std::runtime_error("Buffer::Initialize: try to initialize a buffer with an empty list");
	}
	if (properties & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
		_gpu = true;
	}
	_size = sizeof(list[0]) * list.size();

	//Create a buffer
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = _size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(_vk->device, &bufferInfo, nullptr, &_buffer) != VK_SUCCESS) {
		throw std::runtime_error("Buffer::Initialize: failed to create buffer");
	}

	//Create the buffer memory
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(_vk->device, _buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(_vk->device, &allocInfo, nullptr, &_bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("Buffer::Initialize: failed to allocate buffer memory!");
	}

	//Bind memory with the buffer
	vkBindBufferMemory(_vk->device, _buffer, _bufferMemory, 0);

	Fill<T>(list);
}
*/

/*
template <typename T>
void Buffer::Fill(std::vector<T> list) {
	//Defensive programming for debug
	#ifndef NDEBUG
		if (list.size() == 0) {
			throw std::runtime_error("Buffer::Fill: try to initialize a buffer with an empty list");
		}
		if (!_init) {
			throw std::runtime_error("Buffer::Fill: try to fill a non initialize buffer");
		}
		if (_size != sizeof(list[0]) * list.size()) {
			throw std::runtime_error("Buffer::Fill: try to fill a buffer with different size of the initialized");
		}
	#endif // DEBUG

	if (_gpu) {
		//copy the verteces to the buffer
		void* data;
		vkMapMemory(_vk->device, _bufferMemory, 0, _size, 0, &data);
		memcpy(data, list.data(), (size_t)_size);
		vkUnmapMemory(_vk->device, _bufferMemory);
	}
	else {
		Buffer stagingBuffer(_vk);
		stagingBuffer.Initialize(list, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		//copy the indices to the stagging buffer
		void* data;
		vkMapMemory(_vk->device, stagingBuffer.GetBufferMemory(), 0, _size, 0, &data);
		memcpy(data, list.data(), (size_t)_size);
		vkUnmapMemory(_vk->device, stagingBuffer.GetBufferMemory());

		CopyBuffer cp(_vk);
		//copy from the staging buffer to the index buffer on HIGH performance memory
		cp.Copy(stagingBuffer.GetBuffer(), _buffer, _size);
	}
}
*/


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