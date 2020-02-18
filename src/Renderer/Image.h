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
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags);

};

