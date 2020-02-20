#include "Texture.h"

#include "../VulkanInstance.h"
#include "Buffer.h"
#include "Utils/CopyBuffer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Texture::Texture() : _init(false) {
	
}
Texture::~Texture() {

	vkDestroySampler(VulkanInstance::GetInstance().device, _imageSampler, nullptr);
	vkDestroyImageView(VulkanInstance::GetInstance().device, _imageView, nullptr);

	vkDestroyImage(VulkanInstance::GetInstance().device, _image, nullptr);
	vkFreeMemory(VulkanInstance::GetInstance().device, _imageMemory, nullptr);
}

void Texture::Initialize(std::string pathTexture) {
	/* TODO: ALLOW DIFFERENT IMAGE FORMATS */
	//load the image
	stbi_uc* pixels = stbi_load(pathTexture.c_str(), &_width, &_height, &_channels, STBI_rgb_alpha);
	VkDeviceSize imageSize = _width * _height * 4;

	if (!pixels) {
		throw std::runtime_error("Texture::Initialize: failed to load texture image!");
	}

	//Create a buffer to copy the image
	Buffer stagingBuffer;
	stagingBuffer.Initialize(pixels, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	//Free the image loaded
	stbi_image_free(pixels);

	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	//Can be from 1D, 2D or 3D
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = static_cast<uint32_t>(_width);
	imageInfo.extent.height = static_cast<uint32_t>(_height);
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;

	imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0; // Optional

	if (vkCreateImage(VulkanInstance::GetInstance().device, &imageInfo, nullptr, &_image) != VK_SUCCESS) {
		throw std::runtime_error("Texture::Initialize: failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(VulkanInstance::GetInstance().device, _image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(VulkanInstance::GetInstance().device, &allocInfo, nullptr, &_imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("Texture::Initialize: failed to allocate image memory!");
	}

	vkBindImageMemory(VulkanInstance::GetInstance().device, _image, _imageMemory, 0);

	CopyBuffer cp;

	cp.CopyBufferToImage(stagingBuffer.GetBuffer(), _image, static_cast<uint32_t>(_width), static_cast<uint32_t>(_height));


	CreateImageView();

	CreateTextureSampler();
}

VkImage& Texture::GetImage() {
	return _image;
}

VkDeviceMemory& Texture::GetImageMemory() {
	return _imageMemory;
}


VkImageView& Texture::GetImageView() {
	return _imageView;
}

VkSampler& Texture::GetImageSampler() {
	return _imageSampler;
}


/*
	Finde the memory type use it for the vertex buffer
*/
uint32_t Texture::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
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

void Texture::CreateImageView() {
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = _image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(VulkanInstance::GetInstance().device, &viewInfo, nullptr, &_imageView) != VK_SUCCESS) {
		throw std::runtime_error("Texture::CreateImageView: failed to create texture image view!");
	}
}

void Texture::CreateTextureSampler() {
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;

	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;

	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;

	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	//FOR MIPMAPING, TODO
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(VulkanInstance::GetInstance().device, &samplerInfo, nullptr, &_imageSampler) != VK_SUCCESS) {
		throw std::runtime_error("Texture::CreateTextureSampler: failed to create texture sampler!");
	}
}