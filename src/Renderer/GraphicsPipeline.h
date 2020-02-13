#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <string>

class VulkanInstance;
class Shader;
class RenderPass;
class DescriptorSetLayout;
/*
	Class to generaliza a Graphic Pipeline an its configuration, to be used by materials and other render objects

	@auhtor Fernando del Molino
*/
class GraphicsPipeline
{
private:
	VulkanInstance* _vk;

	std::vector<Shader*> _shaders;

	RenderPass* _renderPass;

	DescriptorSetLayout* _descriptorSetLayout;

	VkPipelineLayout _pipelineLayout;

	VkPipeline _graphicsPipeline;

public:
	GraphicsPipeline(VulkanInstance* vk);

	~GraphicsPipeline();

	/*
		Initialize a graphics pipeline with two vertex and a format. Shaders can be deleted after initialization.

		TODO: more configuration
	*/
	void Initialize(Shader* vertex, Shader* fragment, VkFormat format,
		VkViewport viewport, VkRect2D scissor);

	RenderPass* GetRenderPass() {
		return _renderPass;
	}

	DescriptorSetLayout* GetDescriptorSetLayout() {
		return _descriptorSetLayout;
	}

	VkPipelineLayout& GetPipelineLayout() {
		return _pipelineLayout;
	}

	VkPipeline& GetPipeline() {
		return _graphicsPipeline;
	}
};

