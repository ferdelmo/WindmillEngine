#include "DescriptorSetLayout.h"

DescriptorSetLayout::DescriptorSetLayout() : _descriptorSetLayout(nullptr) {

}

DescriptorSetLayout::~DescriptorSetLayout() {
	vkDestroyDescriptorSetLayout(VulkanInstance::GetInstance().device, _descriptorSetLayout, nullptr);
}