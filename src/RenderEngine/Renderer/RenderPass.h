#pragma once
#include <vulkan/vulkan.h>

class VulkanInstance;
class Image;

/*
	Class to respresent a RenderPass. 
	TODO: Make it configurable by functions or struct

	@author Fernando del Molino
*/
class RenderPass
{
private:
	VkRenderPass _renderPass;

	Image* _depthImage;

	// Used for MSAA
	Image* _colorImage;

public:
	RenderPass();

	~RenderPass();

	/*
		Initialize the renderPass with a format and a image to use as depth
	*/
	void Initialize();

	VkRenderPass& GetRenderPass();

	Image* GetDepthImage();

	Image* GetColorImage();

	void CreateDepthResources();

	void CreateMSAAResources();
};

