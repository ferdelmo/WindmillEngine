#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "../VulkanInstance.h"


class Image
{
private:
	VkImage _image;
	VkDeviceMemory _imageMemory;
	VkImageView _imageView;

	VkFormat _format;
	
	int _width, _height;

	void CreateImageView(VkImageAspectFlags aspectFlags);

	/*
		Maybe externalize this function to create an image in any site
	*/
	void CreateImage(const int width, const int height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
public:

	Image();

	
	~Image();

	/*
		Initialize the image
	*/
	void Initialize(const int width, const int height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags);


	// GETERS 

	VkImage& GetImage();

	VkDeviceMemory& GetImageMemory();

	VkImageView& GetImageView();

	VkFormat& GetFormat();

	static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	/*
		Return one of the suitable candidates
	*/
	static VkFormat FindSupportedFormat(const std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags features);


	/*
		Maybe externalize this function to create an image in any site
	*/
	static Image* CreateImage(const int width, const int height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags, 
		VkSampleCountFlagBits numSample = VK_SAMPLE_COUNT_1_BIT);

	int GetWidth() const {
		return _width;
	}

	int GetHeight() const {
		return _height;
	}

};

