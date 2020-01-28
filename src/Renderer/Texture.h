#pragma once

#include <vulkan/vulkan.h>
#include <exception>
#include <stdexcept>

#include <string>

class VulkanInstance;
/*
*	Class to represent a texture, it contain the image and the memory where is allocated.
*	Can be initialized with a path to a image. 
	-------------------------------------------------------------------------------------
		TODO: do this class more configurablee
	-------------------------------------------------------------------------------------

*	@author Fernando del Molino
*/
class Texture
{
private:

	//image
	VkImage _image;

	//memory to allocate the image
	VkDeviceMemory _imageMemory;

	//image view for the texture
	VkImageView _imageView;

	//texture sampler
	VkSampler _imageSampler;

	//vlikan instance
	VulkanInstance* _vk;

	//bool to check if the buffer is initialized
	bool _init = false;

	int _width = 0, _height = 0, _channels = 0;

	void CreateImageView();

	void CreateTextureSampler();

public:

	//default constructor
	Texture();

	//constructor with the vulkan instance used
	Texture(VulkanInstance* vk);

	~Texture();

	//Initialize the texture by loading the texture in the path
	void Initialize(std::string pathTexture);

	VkImage& GetImage();

	VkDeviceMemory& GetImageMemory();

	VkImageView& GetImageView();

	VkSampler& GetImageSampler();

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

