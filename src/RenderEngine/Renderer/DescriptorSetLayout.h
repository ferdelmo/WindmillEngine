#pragma once
#include <vulkan/vulkan.h>
#include "../VulkanInstance.h"
#include <exception>
#include <stdexcept>
#include "Uniform.h"



struct DescriptorSetLayoutBinding {
public:
    uint16_t binding = 0;
    VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uint16_t descriptorCount = 1;
    VkShaderStageFlags stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    static DescriptorSetLayoutBinding GetUniform(uint32_t binding, uint16_t count, VkShaderStageFlags stageFlags) {
        DescriptorSetLayoutBinding aux = {};
        aux.binding = binding;
        aux.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        aux.descriptorCount = count;
        aux.stageFlags = stageFlags;
        return aux;
    }

    static DescriptorSetLayoutBinding GetImageSampler(uint32_t binding, uint16_t count, VkShaderStageFlags stageFlags) {
        DescriptorSetLayoutBinding aux = {};
        aux.binding = binding;
        aux.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        aux.descriptorCount = count;
        aux.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        return aux;
    }
};

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
        VkDescriptorSetLayoutBinding bindings[10];
        int count = bindingsVector.size();
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
        layoutInfo.pBindings = bindings;

        if (vkCreateDescriptorSetLayout(VulkanInstance::GetInstance().device, &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("DescriptorSetLayout::Initialize: failed to create descriptor set layout!");
        }
	}

    VkDescriptorSetLayout& GetDescriptor() {
        return _descriptorSetLayout;
    }
};

