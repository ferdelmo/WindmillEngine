#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class VulkanInstance;

/*
	TODO: ALLOW DIFFERENT TYPE OF MEMORY TO USE, COPYNG TO GPU MEMORY WITH 
	STAGGING BUFFER

	TODO: UTIL CLASS WITH COMMAND BUFFER TO ALLOW COPY BETWEEN CPU AND GPU MEMORY
*/

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

	//Reference to the VulkanInstance (device, queues, etc)
	VulkanInstance* _vk;

	//Size of the buffer
	VkDeviceSize _size;

	//boolean to chek if the buffer use gpu memory or local
	bool _gpu = false;
public:

	/*
		Default constructor
	*/
	Buffer(VulkanInstance* vk);

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
		VkMemoryPropertyFlags properties);

	/*
		Fill the vertex with the array pased
	*/
	template <typename T>
	void Fill(std::vector<T> list);

	/*
		Return the vulkan buffer of the instance
	*/
	VkBuffer GetBuffer() const;

	/*
		Return the vulkan buffer memory of the instance
	*/
	VkDeviceMemory GetBufferMemory() const;

	/*
		Find types of memory suported to check in the buffer creation
	*/
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

