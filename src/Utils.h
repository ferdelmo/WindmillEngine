#pragma once

#include "vulkan/vulkan.h"
#include <stdexcept>
#include <array>


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;

	/*
		bind the data to the data vertex shader receive
	*/
	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		//VK_VERTEX_INPUT_RATE_INSTANCE -> move the data entry after each instance for instanced rendering
		return bindingDescription;
	}
	/*
		describes how to extrat a vertex attribute from a chunk of vertex data
	*/
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		/*
			FORMAT:
			float: VK_FORMAT_R32_SFLOAT
			vec2: VK_FORMAT_R32G32_SFLOAT
			vec3: VK_FORMAT_R32G32B32_SFLOAT
			vec4: VK_FORMAT_R32G32B32A32_SFLOAT
		*/

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};


class Buffer {
private:
	//Buffer
	VkBuffer _buffer;
	//Buffer memory;
	VkDeviceMemory _bufferMemory;

	VkDevice _device;

	bool _init = false;

	/*
		Find the index to the memory type that has the required type and properties in the physical device
	*/
	static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, 
		const VkPhysicalDevice& physicalDevice) {

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) &&
				(memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	/*
	   Creates a buffer following a specification
	*/
	static void createBuffer(VkDevice device, VkDeviceSize size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkBuffer& buffer,
			VkDeviceMemory& bufferMemory) {

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

	/*
		Copy buffer
	*/
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		/*
			MAYBE CREATE A POOL OF COMMNADS OF THIS TYPE INSTEAD OF ONE TEMPORAL
		*/
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		//Allocate the command buffer
		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		//Start recording commands
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = 0; // Optional
		copyRegion.dstOffset = 0; // Optional
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		//Stop recording commands
		vkEndCommandBuffer(commandBuffer);

		//Submit the commandbuffer
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		// instead of wait for the command to end, maybe wait for a fence to schedule multiple transfers
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	}

public:
	/*
		Create an empty buffer
	*/
	Buffer() : _init(false) {

	}

	/*
		Create a buffer with the specifications
	*/
	Buffer(VkDevice device) : _device(device), _init(false) {
	
	}

	~Buffer() {
		Destroy();
	}

	/*
		Destroy the buffer and free the memory
	*/
	bool Destroy() {
		if (_init) {
			vkDestroyBuffer(_device, _buffer, nullptr);
			vkFreeMemory(_device, _bufferMemory, nullptr);
		}
		_init = false;
		return true;
	}

	bool Initialize(const VkDeviceSize& size,
		const VkBufferUsageFlags& usage,
		const VkMemoryPropertyFlags& properties,
		VkBuffer& buffer,
		VkDeviceMemory& bufferMemory) {
		
		//If alredy initialized, clear the last buffer or return error
		if (!_init) {
			Destroy();
		}

		_init = true;
		return true;
	}

	//return the buffer
	VkBuffer& GetBuffer() {
		if (!_init) {
			throw std::runtime_error("trying to access an unintialized buffer");
		}
		return _buffer;
	}

	//return the memory of the buffer
	VkDeviceMemory& GetBufferMemory(){
		if (!_init) {
			throw std::runtime_error("trying to access an unintialized buffer");
		}
		return _bufferMemory;
	}
};