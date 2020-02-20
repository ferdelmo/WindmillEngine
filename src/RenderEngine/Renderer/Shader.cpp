#include "Shader.h"

#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>

#include "../VulkanInstance.h"
#include "Uniform.h"

Shader::Shader() :  _shaderModule(nullptr), _descriptorSetLayoutBindings(0)
{

}

Shader::~Shader() {
    vkDestroyShaderModule(VulkanInstance::GetInstance().device, _shaderModule, nullptr);
}


void Shader::Initialize(const std::string path, VkShaderStageFlagBits stage, std::vector<UniformInfo*> descriptorSetLayoutBindings) {
    CreateShaderModule(path);
    _descriptorSetLayoutBindings = descriptorSetLayoutBindings;
    _stage = stage;
}

std::vector<UniformInfo*>& Shader::GetDescriptorSetLayoutBindings() {
    return _descriptorSetLayoutBindings;
}

void Shader::CreateShaderModule(const std::string& path) {
    std::vector<char> code = ReadFile(path);

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(VulkanInstance::GetInstance().device, &createInfo, nullptr, &_shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Shader::Initialize: failed to create shader module!");
    }
}

std::vector<char> Shader::ReadFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Shader::Initialize: failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

VkShaderModule& Shader::GetShaderModule() {
    return _shaderModule;
}

std::string Shader::GetEntryName() {
    return _entryName;
}

VkShaderStageFlagBits& Shader::GetStage() {
    return _stage;
}