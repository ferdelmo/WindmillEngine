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
        std::vector<VkDescriptorSetLayoutBinding> bindings(0);
        for (int i = 0; i < count; i++) {
            if (bindingsVector[i]->uniformType != UniformTypes::CONSTANT) {
                VkDescriptorSetLayoutBinding aux = {};
                aux.binding = bindingsVector[i]->binding;
                aux.descriptorType = bindingsVector[i]->type;
                aux.descriptorCount = bindingsVector[i]->descriptorCount;
                aux.stageFlags = bindingsVector[i]->stageFlags;
                aux.pImmutableSamplers = nullptr; // Optional
                bindings.push_back(aux);
            }
        }
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(VulkanInstance::GetInstance().device, &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("DescriptorSetLayout::Initialize: failed to create descriptor set layout!");
        }
	}

    VkDescriptorSetLayout& GetDescriptor() {
        return _descriptorSetLayout;
    }
};

