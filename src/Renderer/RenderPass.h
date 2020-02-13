#pragma once
#include <vulkan/vulkan.h>

class VulkanInstance;

/*
	Class to respresent a RenderPass. 
	TODO: Make it configurable by functions or struct

	@author Fernando del Molino
*/
class RenderPass
{
private:
	VulkanInstance* _vk;

	VkRenderPass _renderPass;

public:
	RenderPass(VulkanInstance* vk);

	~RenderPass();

	void Initialize(VkFormat format);

	VkRenderPass& GetRenderPass();
};

