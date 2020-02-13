#include "DescriptorSetLayout.h"

DescriptorSetLayout::DescriptorSetLayout(VulkanInstance* vk) : _vk(vk), _descriptorSetLayout(nullptr) {

}

DescriptorSetLayout::~DescriptorSetLayout() {
	vkDestroyDescriptorSetLayout(_vk->device, _descriptorSetLayout, nullptr);
}