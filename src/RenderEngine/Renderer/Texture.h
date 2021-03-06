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

	CHANGE THIS CLASS FOR A TEXTURE WITH A IMAGE, TO REUTILIZE CODE

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

	//bool to check if the buffer is initialized
	bool _init = false;

	int _width = 0, _height = 0, _channels = 0;

	std::string _path = "";

	void CreateImageView();

	void CreateTextureSampler();

public:

	//default constructor
	Texture();

	~Texture();

	//Initialize the texture by loading the texture in the path
	void Initialize(std::string pathTexture);

	//Change the image of the texture, for now mipmap, etc. cannot be changed
	void ChangeSourceImage(std::string pathTexture);


	VkImage& GetImage();

	VkDeviceMemory& GetImageMemory();

	VkImageView& GetImageView();

	VkSampler& GetImageSampler();

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	std::string GetPath() const;
};

