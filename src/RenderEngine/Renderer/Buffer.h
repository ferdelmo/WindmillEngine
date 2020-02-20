#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <exception>
#include <stdexcept>

#include "../VulkanInstance.h"
#include "Utils/CopyBuffer.h"

class VulkanInstance;

/**
*	Represent a buffer that can be innstanciated with different types of memory, used for
*	the vertexBuffer, fragmentBuffer, etc.
*	
*	It can be used to represent different type of buffers, for example, those with memory
*	in CPU or in GPU
*	@author Fernando del Molino
*/
class Buffer
{

private:
	//Buffer
	VkBuffer _buffer;
	//Buffer memory
	VkDeviceMemory _bufferMemory;

	//bool to check if the buffer is initialized
	bool _init = false;

	//Size of the buffer
	VkDeviceSize _size;

	//boolean to chek if the buffer use gpu memory or local
	bool _gpu = false;

public:
	bool isVertex = false;

public:

	/*
		Default constructor
	*/
	Buffer();

	/*
		Destructor for the buffer
	*/
	~Buffer();

	/*
		Initialize the buffer with the arguments specified
	*/
	template <typename T>
	void Initialize(const std::vector<T>& list,
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

		if (vkCreateBuffer(VulkanInstance::GetInstance().device, &bufferInfo, nullptr, &_buffer) != VK_SUCCESS) {
			throw std::runtime_error("Buffer::Initialize: failed to create buffer");
		}

		//Create the buffer memory
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(VulkanInstance::GetInstance().device, _buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(VulkanInstance::GetInstance().device, &allocInfo, nullptr, &_bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("Buffer::Initialize: failed to allocate buffer memory!");
		}

		//Bind memory with the buffer
		vkBindBufferMemory(VulkanInstance::GetInstance().device, _buffer, _bufferMemory, 0);
		_init = true;
		this->Fill(list);
	}

	/*
		Fill the vertex with the array pased
	*/
	template <typename T>
	void Fill(std::vector<T> list) {
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

		if (!_gpu) {
			//copy the verteces to the buffer
			void* data;
			vkMapMemory(VulkanInstance::GetInstance().device, _bufferMemory, 0, _size, 0, &data);
			memcpy(data, list.data(), (size_t)_size);
			vkUnmapMemory(VulkanInstance::GetInstance().device, _bufferMemory);
		}
		else {
			Buffer stagingBuffer;
			stagingBuffer.Initialize(list, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			//copy the indices to the stagging buffer
			void* data;
			vkMapMemory(VulkanInstance::GetInstance().device, stagingBuffer.GetBufferMemory(), 0, _size, 0, &data);
			memcpy(data, list.data(), (size_t)_size);
			vkUnmapMemory(VulkanInstance::GetInstance().device, stagingBuffer.GetBufferMemory());

			CopyBuffer cp;
			//copy from the staging buffer to the index buffer on HIGH performance memory
			cp.Copy(stagingBuffer.GetBuffer(), _buffer, _size);
		}
	}


	/*
		Initialize the buffer with the arguments specified
	*/
	template <typename T>
	void Initialize(const T* data,
		VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties) {

		if (data == nullptr) {
			throw std::runtime_error("Buffer::Initialize: try to initialize a buffer with an empty list");
		}
		if (properties & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
			_gpu = true;
		}
		_size = size;

		//Create a buffer
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(VulkanInstance::GetInstance().device, &bufferInfo, nullptr, &_buffer) != VK_SUCCESS) {
			throw std::runtime_error("Buffer::Initialize: failed to create buffer");
		}

		//Create the buffer memory
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(VulkanInstance::GetInstance().device, _buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(VulkanInstance::GetInstance().device, &allocInfo, nullptr, &_bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("Buffer::Initialize: failed to allocate buffer memory!");
		}

		//Bind memory with the buffer
		vkBindBufferMemory(VulkanInstance::GetInstance().device, _buffer, _bufferMemory, 0);
		_init = true;
		this->Fill(data, size);
	}

	/*
		Fill the vertex with the array pased
	*/
	template <typename T>
	void Fill(T* data, VkDeviceSize size) {
		//Defensive programming for debug
		#ifndef NDEBUG
			if (data == nullptr) {
				throw std::runtime_error("Buffer::Fill: try to initialize a buffer with an empty list");
			}
			if (!_init) {
				throw std::runtime_error("Buffer::Fill: try to fill a non initialize buffer");
			}
			if (_size != size) {
				throw std::runtime_error("Buffer::Fill: try to fill a buffer with different size of the initialized");
				}
		#endif // DEBUG

		if (!_gpu) {
			//copy the verteces to the buffer
			void* ptrdata;
			vkMapMemory(VulkanInstance::GetInstance().device, _bufferMemory, 0, _size, 0, &ptrdata);
			memcpy(ptrdata, data, (size_t)_size);
			vkUnmapMemory(VulkanInstance::GetInstance().device, _bufferMemory);
		}
		else {
			Buffer stagingBuffer;
			stagingBuffer.Initialize(data, _size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			//copy the indices to the stagging buffer
			void* ptrdata;
			vkMapMemory(VulkanInstance::GetInstance().device, stagingBuffer.GetBufferMemory(), 0, _size, 0, &ptrdata);
			memcpy(ptrdata, data, (size_t)_size);
			vkUnmapMemory(VulkanInstance::GetInstance().device, stagingBuffer.GetBufferMemory());

			CopyBuffer cp;
			//copy from the staging buffer to the index buffer on HIGH performance memory
			cp.Copy(stagingBuffer.GetBuffer(), _buffer, _size);
		}
	}

	/*
		Return the vulkan buffer of the instance
	*/
	VkBuffer& GetBuffer();

	/*
		Return the vulkan buffer memory of the instance
	*/
	VkDeviceMemory& GetBufferMemory();

	/*
		Find types of memory suported to check in the buffer creation
	*/
	static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

