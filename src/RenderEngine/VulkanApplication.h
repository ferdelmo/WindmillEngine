#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>
#include <chrono>

#include "VulkanInstance.h"
#include "Renderer/Buffer.h"
#include "Renderer/StaticMesh.h"
#include "Renderer/Vertex.h"
#include "Renderer/Camera.h"
#include "Renderer/GraphicsPipeline.h"
#include "Renderer/Shader.h"
#include "Renderer/DescriptorSetLayout.h"
#include "Renderer/RenderPass.h"
#include "Renderer/Image.h"

const int WIDTH = 1920;
const int HEIGHT = 1080;

const int MAX_FRAMES_IN_FLIGHT = 2;
/*
const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
    {{-0.5f, -0.5f, 1.0f}, {1.0f, 1.0f, 0.0f}},
    {{0.5f, -0.5f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{0.5f, 0.5f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f, 1.0f}, {0.5f, 0.5f, 0.5f}}
};

const std::vector<uint16_t> indices = {
    0, 2, 1, 3, 2, 0, 4, 5, 6, 6, 7, 4,
    0, 4, 3, 4, 7, 3, 3, 7, 2, 7, 6, 2,
    6, 5, 2, 5, 1, 2, 5, 4, 1, 4, 0, 1
};

*/
const std::vector<Vertex> vertices1 = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
};

const std::vector<uint16_t> indices1 = {
    0, 1, 2, 2, 3, 0
};
const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};

class VulkanApplication {
public:
    

    void run() {
        initVulkan();
        mainLoop();
        cleanup();
    }

private:

    /* for now this shouldnt exists, until the vulkan instance is more configurable and less hardcoded */

    VulkanInstance* vk;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    RenderPass* renderPass;
    GraphicsPipeline* pipeline;

    Shader* vertex;
    Shader* fragment;

    Image* depthImage;

    VkCommandPool commandPool;

    Camera cam;

    StaticMesh* mesh;
    StaticMesh* mesh1;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    void initVulkan() {
        vk = &VulkanInstance::GetInstance();


        createSwapChain();
        createImageViews();

        pipeline = new GraphicsPipeline();
        vertex = new Shader();
        std::vector<DescriptorSetLayoutBinding> vertexDescriptor(0);
        vertexDescriptor.push_back(DescriptorSetLayoutBinding::GetUniform(0, 1, VK_SHADER_STAGE_VERTEX_BIT));
        vertex->Initialize("../resources/shaders/vert.spv", VK_SHADER_STAGE_VERTEX_BIT, vertexDescriptor);

        fragment = new Shader();
        std::vector<DescriptorSetLayoutBinding> fragmentDescriptor(0);
        fragmentDescriptor.push_back(DescriptorSetLayoutBinding::GetImageSampler(1, 1, VK_SHADER_STAGE_FRAGMENT_BIT));
        fragment->Initialize("../resources/shaders/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, fragmentDescriptor);


        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapChainExtent.width;
        viewport.height = (float)swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = swapChainExtent;

        pipeline->Initialize(renderPass, vertex, fragment, swapChainImageFormat, viewport, scissor);

        createFramebuffers();

        cam = Camera(glm::vec3(-2.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 60, 16 / 9.0f, 0.1f, 100.0f);

        mesh = new StaticMesh(vertices, indices, "../resources/textures/texture.jpg", pipeline);
        mesh1 = new StaticMesh(vertices1, indices1, "../resources/textures/texture.jpg", pipeline);

        mesh->SetCamera(cam);
        mesh1->SetCamera(cam);

        mesh->Initialize();
        mesh1->Initialize();

        createCommandPool();
        createCommandBuffers();
        createSyncObjects();
    }

    void mainLoop() {

        auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        while (!glfwWindowShouldClose(VulkanInstance::GetInstance().window)) {
            glfwPollEvents();
            currentTime = std::chrono::high_resolution_clock::now(); 
            float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
            mesh->Update(time);
            mesh1->Update(time);
            startTime = std::chrono::high_resolution_clock::now();
            drawFrame();
            
        }

        vkDeviceWaitIdle(VulkanInstance::GetInstance().device);
    }

    void cleanupSwapChain() {
        for (auto framebuffer : swapChainFramebuffers) {
            vkDestroyFramebuffer(VulkanInstance::GetInstance().device, framebuffer, nullptr);
        }

        vkFreeCommandBuffers(VulkanInstance::GetInstance().device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());


        for (auto imageView : swapChainImageViews) {
            vkDestroyImageView(VulkanInstance::GetInstance().device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(VulkanInstance::GetInstance().device, swapChain, nullptr);
    }

    void cleanup() {
        cleanupSwapChain();

        //delete mesh;
        delete mesh1;
        delete pipeline;
        /*
        vkDestroyBuffer(*vk.device, *indexBuffer.GetBuffer(), nullptr);
        vkFreeMemory(*vk.device, *indexBuffer.GetBufferMemory(), nullptr);

        vkDestroyBuffer(*vk.device, *vertexBuffer.GetBuffer(), nullptr);
        vkFreeMemory(*vk.device, *vertexBuffer.GetBufferMemory(), nullptr);
        */
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(VulkanInstance::GetInstance().device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(VulkanInstance::GetInstance().device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(VulkanInstance::GetInstance().device, inFlightFences[i], nullptr);
        }

        vkDestroyCommandPool(VulkanInstance::GetInstance().device, commandPool, nullptr);
    }

    void recreateSwapChain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(VulkanInstance::GetInstance().window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(VulkanInstance::GetInstance().window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(VulkanInstance::GetInstance().device);

        cleanupSwapChain();

        createSwapChain();
        createImageViews();
        createFramebuffers();
        createCommandBuffers();
    }

    void createSwapChain() {
        VulkanInstance::SwapChainSupportDetails swapChainSupport = VulkanInstance::GetInstance().QuerySwapChainSupport(VulkanInstance::GetInstance().physicalDevice);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = VulkanInstance::GetInstance().surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        VulkanInstance::QueueFamilyIndices indices = VulkanInstance::GetInstance().FindQueueFamilies(VulkanInstance::GetInstance().physicalDevice);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        if (vkCreateSwapchainKHR(VulkanInstance::GetInstance().device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(VulkanInstance::GetInstance().device, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(VulkanInstance::GetInstance().device, swapChain, &imageCount, swapChainImages.data());

        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }

    void createImageViews() {
        swapChainImageViews.resize(swapChainImages.size());

        for (size_t i = 0; i < swapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(VulkanInstance::GetInstance().device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views!");
            }
        }
    }

    void createFramebuffers() {
        swapChainFramebuffers.resize(swapChainImageViews.size());

        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            std::array<VkImageView, 2> attachments = {
                swapChainImageViews[i],
                depthImage->GetImageView()
            };

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass->GetRenderPass();
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(VulkanInstance::GetInstance().device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    void createCommandPool() {
        VulkanInstance::QueueFamilyIndices queueFamilyIndices = VulkanInstance::GetInstance().FindQueueFamilies(VulkanInstance::GetInstance().physicalDevice);

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(VulkanInstance::GetInstance().device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics command pool!");
        }
    }

    void createCommandBuffers() {
        commandBuffers.resize(swapChainFramebuffers.size());

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

        if (vkAllocateCommandBuffers(VulkanInstance::GetInstance().device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for (size_t i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass->GetRenderPass();
            renderPassInfo.framebuffer = swapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = swapChainExtent;

            /* Clear the 2 attachments, color and depth */
            std::array<VkClearValue, 2> clearValues = {};
            clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipeline());

            mesh1->BindCommandsToBuffer(commandBuffers[i]);

            mesh->BindCommandsToBuffer(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);

            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
    }

    void createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(VulkanInstance::GetInstance().device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(VulkanInstance::GetInstance().device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(VulkanInstance::GetInstance().device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
    }

    void drawFrame() {
        vkWaitForFences(VulkanInstance::GetInstance().device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(VulkanInstance::GetInstance().device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(VulkanInstance::GetInstance().device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        imagesInFlight[imageIndex] = inFlightFences[currentFrame];

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(VulkanInstance::GetInstance().device, 1, &inFlightFences[currentFrame]);

        if (vkQueueSubmit(VulkanInstance::GetInstance().graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(VulkanInstance::GetInstance().presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || VulkanInstance::GetInstance().framebufferResized) {
            VulkanInstance::GetInstance().framebufferResized = false;
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        }
        else {
            int width, height;
            glfwGetFramebufferSize(VulkanInstance::GetInstance().window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }
};