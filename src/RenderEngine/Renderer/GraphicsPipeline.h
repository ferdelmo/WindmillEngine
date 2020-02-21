#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <string>

class VulkanInstance;
class Shader;
class RenderPass;
class DescriptorSetLayout;
class Image;

/*
	Class to generaliza a Graphic Pipeline an its configuration, to be used by materials and other render objects

	@auhtor Fernando del Molino
*/
class GraphicsPipeline
{
private:
	std::vector<Shader*> _shaders;

	DescriptorSetLayout* _descriptorSetLayout;

	VkPipelineLayout _pipelineLayout;

	VkPipeline _graphicsPipeline;

public:
	GraphicsPipeline();

	~GraphicsPipeline();

	/*
		Initialize a graphics pipeline with two vertex and a format. Shaders can be deleted after initialization.

		It assumes that the renderPass has a depthImage configured
		TODO: more configuration
	*/
	void Initialize(RenderPass* renderPass, Shader* vertex, Shader* fragment, VkFormat format,
		VkVertexInputBindingDescription inputBind,
		std::vector<VkVertexInputAttributeDescription> inputAttr,
		VkViewport viewport, VkRect2D scissor);

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

