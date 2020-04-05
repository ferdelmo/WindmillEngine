#include "Image.h"
#include <stdexcept>


Image::Image() {

}

Image::~Image() {
	vkDestroyImageView(VulkanInstance::GetInstance().device, _imageView, nullptr);

	vkDestroyImage(VulkanInstance::GetInstance().device, _image, nullptr);
	vkFreeMemory(VulkanInstance::GetInstance().device, _imageMemory, nullptr);
}
	
void Image::Initialize(const int width, const int height, VkFormat format, VkImageTiling tiling,
	VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags) {

	_format = format;

	CreateImage(width, height, format, tiling, usage, properties);
	CreateImageView(aspectFlags);
}

VkImage& Image::GetImage() {
	return _image;
}

VkDeviceMemory& Image::GetImageMemory() {
	return _imageMemory;
}

VkImageView& Image::GetImageView() {
	return _imageView;
}
VkFormat& Image::GetFormat() {
	return _format;
}

VkFormat Image::FindSupportedFormat(const std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(VulkanInstance::GetInstance().physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw std::runtime_error("Image::FindSupportedFormat: failed to find supported format!");
}

void Image::CreateImageView(VkImageAspectFlags aspectFlags) {
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = _image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = _format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(VulkanInstance::GetInstance().device, &viewInfo, nullptr, &_imageView) != VK_SUCCESS) {
		throw std::runtime_error("Texture::CreateImageView: failed to create texture image view!");
	}
}

void Image::CreateImage(const int width, const int height, VkFormat format, VkImageTiling tiling,
	VkImageUsageFlags usage, VkMemoryPropertyFlags properties) {

	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(VulkanInstance::GetInstance().device, &imageInfo, nullptr, &_image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(VulkanInstance::GetInstance().device, _image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(VulkanInstance::GetInstance().device, &allocInfo, nullptr, &_imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(VulkanInstance::GetInstance().device, _image, _imageMemory, 0);
}

Image* Image::CreateImage(const int width, const int height, VkFormat format, VkImageTiling tiling,
	VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags,
	VkSampleCountFlagBits numSample) {
	Image* resul = new Image();

	resul->_format = format;

	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = numSample;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(VulkanInstance::GetInstance().device, &imageInfo, nullptr, &resul->_image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(VulkanInstance::GetInstance().device, resul->_image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(VulkanInstance::GetInstance().device, &allocInfo, nullptr, &resul->_imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(VulkanInstance::GetInstance().device, resul->_image, resul->_imageMemory, 0);

	resul->CreateImageView(aspectFlags);

	return resul;
}

/*
	Finde the memory type use it for the vertex buffer
*/
uint32_t Image::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(VulkanInstance::GetInstance().physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) &&
			(memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("Texture::FindMemoryType: failed to find suitable memory type!");
}