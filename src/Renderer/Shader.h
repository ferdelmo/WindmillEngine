#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include "DescriptorSetLayout.h"


class VulkanInstance;

/*
	Abstract class to implement a shader, configurable to be either fragment, vertex or whatever
	maybe information about the description layour should be here

	@author Fernando del Molino
*/
class Shader
{
private:
	VkShaderModule _shaderModule;

	std::vector<DescriptorSetLayoutBinding> _descriptorSetLayoutBindings;

	static std::vector<char> ReadFile(const std::string& path);

	void CreateShaderModule(const std::string& path);
	
	std::string _entryName = "main";

	VkShaderStageFlagBits _stage;

public:

    Shader();

	~Shader();

	void Initialize(const std::string path, VkShaderStageFlagBits stage, std::vector<DescriptorSetLayoutBinding> descriptorSetLayoutBindings);

	std::vector<DescriptorSetLayoutBinding>& GetDescriptorSetLayoutBindings();

	VkShaderModule& GetShaderModule();

	std::string GetEntryName();

	VkShaderStageFlagBits& GetStage();
};

