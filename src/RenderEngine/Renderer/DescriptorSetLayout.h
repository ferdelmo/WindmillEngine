#pragma once
#include <vulkan/vulkan.h>
#include "../VulkanInstance.h"
#include <exception>
#include <stdexcept>
#include "Uniform.h"

/*
    Class to respresent the DecriptorSetLayout and DescriptorSetLayoutBindings, adding
    some static function to generate easily some common layouts bindings. Also, a way 
    to initilize them

    @author Fernando del Molino
*/
class DescriptorSetLayout
{
private:
	VkDescriptorSetLayout _descriptorSetLayout;

public:

    const int MAX_LAYOUT_BINDINGS = 10;
	DescriptorSetLayout();

    ~DescriptorSetLayout();
    
	void Initialize(std::vector<UniformInfo*> bindingsVector) {
        int count = bindingsVector.size();
        std::vector<VkDescriptorSetLayoutBinding> bindings(count);
        for (int i = 0; i < count; i++) {
            bindings[i].binding = bindingsVector[i]->binding;
            bindings[i].descriptorType = bindingsVector[i]->type;
            bindings[i].descriptorCount = bindingsVector[i]->descriptorCount;
            bindings[i].stageFlags = bindingsVector[i]->stageFlags;
            bindings[i].pImmutableSamplers = nullptr; // Optional
        }
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = count;
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(VulkanInstance::GetInstance().device, &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("DescriptorSetLayout::Initialize: failed to create descriptor set layout!");
        }
	}

    VkDescriptorSetLayout& GetDescriptor() {
        return _descriptorSetLayout;
    }
};

