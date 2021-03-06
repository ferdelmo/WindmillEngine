#include "SingleThreadRenderer.h"

#include <array>
#include <iostream>
#include <stdexcept>


#include "RenderEngine/VulkanInstance.h"
#include "RenderEngine/Renderer/Image.h"
#include "RenderEngine/Renderer/RenderPass.h"

#include "RenderEngine/Renderer/Renderizable.h"
#include "RenderEngine/Renderer/StaticMesh.h"

#include "RenderEngine/Renderer/Utils/MaterialUtils.h"
#include "RenderEngine/Renderer/Material.h"
#include "RenderEngine/Renderer/MaterialInstance.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

#include "Engine/World.h"


SingleThreadRenderer::SingleThreadRenderer() : _initialized(false), objects(0), objectsToAdd() {
    updateCommands = std::vector<bool>();
}

SingleThreadRenderer::~SingleThreadRenderer() {

}

SingleThreadRenderer* SingleThreadRenderer::_instance = nullptr;

SingleThreadRenderer& SingleThreadRenderer::GetInstance() {
    if (_instance == nullptr) {
        _instance = new SingleThreadRenderer();
    }
    return *_instance;
}

void SingleThreadRenderer::DrawFrame() {

    bool needUpdate = false;
    /*
        Check objects to add to render
    */
    if (objectsToAdd.size() > 0) {
        for (int i = 0; i < objectsToAdd.size(); i++) {
            objects.push_back(objectsToAdd[i]);
        }

        objectsToAdd.clear();

        for (int auxi = 0; auxi < updateCommands.size(); auxi++) {
            updateCommands[auxi] = true;
        }
    }

    /*
        Check objects to remove from render
    */
    if (objectsToRemove.size() > 0) {

        while (objectsToRemove.size() > 0) {

            std::vector<Renderizable*>::iterator iter;

            bool find = false;
            for (iter = objects.begin(); iter != objects.end(); iter++) {
                if (*(iter) == objectsToRemove.back()) {
                    objectsToRemove.pop_back();
                    find = true;
                    break;
                }
            }

            if (find) {
                objects.erase(iter);

                for (int auxi = 0; auxi < updateCommands.size(); auxi++) {
                    updateCommands[auxi] = true;
                }
            }
            else {
                // the object does not exist
                objectsToRemove.pop_back();
            }
        }
    }




    vkWaitForFences(VulkanInstance::GetInstance().device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(VulkanInstance::GetInstance().device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        RecreateSwapChain();
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

    /*
       Update command buffer if needed
   */
    if (updateCommands[imageIndex]) {
        updateCommands[imageIndex] = false;
        UpdateCommandBuffer(imageIndex);

        // call the delegates when is safe

    }

    std::vector<DeleteInfo>::iterator iter = deleteInfoVector.begin();
    while (iter != deleteInfoVector.end()) {
        std::vector<Renderizable*>::iterator erase = objects.begin();

        while (erase != objects.end()) {
            if (*erase == iter->obj) {
                erase = objects.erase(erase);
                break;
            }
            ++erase;
        }
        UpdateCommandBuffer(imageIndex);
        bool clean = true;

        (*iter).check[imageIndex] = true;

        for (int i = 0; i < (*iter).check.size() && clean; i++) {
            clean = clean && (*iter).check[i];
        }

        if (clean) {
            (*iter).func();
            iter = deleteInfoVector.erase(iter);
        }
        else {
            ++iter;
        }
    }


    vkResetFences(VulkanInstance::GetInstance().device, 1, &inFlightFences[currentFrame]);

    VkResult submit = vkQueueSubmit(VulkanInstance::GetInstance().graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]);
    if (submit != VK_SUCCESS) {
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
        RecreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void SingleThreadRenderer::InitializeSwapChain() {

    CreateSwapChain();

    renderPass = new RenderPass();
    renderPass->Initialize();
    depthRenderPass = RenderPass::GenerateOnlyDepthRenderPass();

    CreateImageViews();
}

void SingleThreadRenderer::Initialize() {


    if (_initialized) {
        throw std::runtime_error("RenderThread::Initizalize: Alredy initialized!");
        return;
    }

    _shadowMaterial = GetShadowMapMaterial();

    _uniforms = _shadowMaterial->GetMaterial()->GenerateDescriptorSet(_descriptorPool, _descriptorSet);

    CreateFramebuffers();
    CreateCommandPool();
    CreateCommandBuffers();
    CreateSyncObjects();

    _initialized = true;
    VkFormat depthFormat = Image::FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    for (int i = 0; i < MAX_LIGHTS; i++) {
        depthImages.push_back(Image::CreateImage(SingleThreadRenderer::GetInstance().GetExtent().width * 2, SingleThreadRenderer::GetInstance().GetExtent().height * 2, 
            depthFormat,
            VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT| VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            VK_IMAGE_ASPECT_DEPTH_BIT, VK_SAMPLE_COUNT_1_BIT));
    }

    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = samplerInfo.addressModeU;
    samplerInfo.addressModeW = samplerInfo.addressModeU;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.maxAnisotropy = 1.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 1.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

    if (vkCreateSampler(VulkanInstance::GetInstance().device, &samplerInfo, nullptr, &depthSampler) != VK_SUCCESS) {
        throw std::runtime_error("Texture::CreateTextureSampler: failed to create texture sampler!");
    }
}

void SingleThreadRenderer::UpdateCommandBuffer(size_t i) {
    if (vkResetCommandBuffer(commandBuffers[i], 0)) {
        throw std::runtime_error("failed to reset the command buffer!");
    }

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    static bool first = true;
    if (first) {
        for (int light = 0; light < MAX_LIGHTS; light++) {
            VkImageSubresourceRange subresourceRange = {};
            subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            subresourceRange.baseMipLevel = 0;
            subresourceRange.levelCount = 1;
            subresourceRange.baseArrayLayer = 0;
            subresourceRange.layerCount = 1;

            // Change image layout of copied face to shader read
            Image::SetImageLayout(
                commandBuffers[i],
                depthImages[light]->GetImage(),
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                subresourceRange);
        }

        first = false;
    }

    // CHANGE DYNAMICALLY DEPENDING ON THE NUMBER OF LIGHTS
    for (int light = 0; light < 2; light++) {

        // The shadow map pass
        VkRenderPassBeginInfo renderPassInfoDepth = {};
        renderPassInfoDepth.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfoDepth.renderPass = depthRenderPass->GetRenderPass();
        renderPassInfoDepth.framebuffer = depthBuffer;
        renderPassInfoDepth.renderArea.offset = { 0, 0 };
        renderPassInfoDepth.renderArea.extent.width = depthRenderPass->GetDepthImage()->GetWidth();
        renderPassInfoDepth.renderArea.extent.height = depthRenderPass->GetDepthImage()->GetHeight();

        /* Clear the 2 attachments, color and depth */
        VkClearValue clearValuesDepth;
        clearValuesDepth.depthStencil = { 1.0f, 0 };

        renderPassInfoDepth.clearValueCount = 1;
        renderPassInfoDepth.pClearValues = &clearValuesDepth;

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfoDepth, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)depthRenderPass->GetDepthImage()->GetWidth();
        viewport.height = (float)depthRenderPass->GetDepthImage()->GetHeight();
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = renderPassInfoDepth.renderArea.extent;


        vkCmdSetViewport(commandBuffers[i], 0, 1, &viewport);

        vkCmdSetScissor(commandBuffers[i], 0, 1, &scissor);

        // Set depth bias (aka "Polygon offset")
        // Required to avoid shadow mapping artefacts

        float depthBiasConstant = 1.25f;

        float depthBiasSlope = 1.75f;
        vkCmdSetDepthBias(
            commandBuffers[i],
            depthBiasConstant,
            0.0f,
            depthBiasSlope);


        // Generate a specific constat pipeline for the shadow map
        //vkCmdBindPipeline(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.offscreen);

        //same with the descriptor layout and descriptor set?
        //vkCmdBindDescriptorSets(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayouts.offscreen, 0, 1, &descriptorSets.offscreen, 0, NULL);


        for (auto& obj : objects) {
            StaticMesh* mesh = (StaticMesh*)obj;
            VkDeviceSize offsets[] = { 0 };

            mesh->BindCommandsToBufferShadow(commandBuffers[i], light);
        }
        /*
        vkCmdBindVertexBuffers(drawCmdBuffers[i], 0, 1, &scenes[sceneIndex].vertices.buffer, offsets);
        vkCmdBindIndexBuffer(drawCmdBuffers[i], scenes[sceneIndex].indices.buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(drawCmdBuffers[i], scenes[sceneIndex].indexCount, 1, 0, 0, 0);
        */

        vkCmdEndRenderPass(commandBuffers[i]);

        // Make sure color writes to the framebuffer are finished before using it as transfer source
        Image::SetImageLayout(
            commandBuffers[i],
            depthRenderPass->GetDepthImage()->GetImage(),
            VK_IMAGE_ASPECT_DEPTH_BIT,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = 1;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount = 1;

        // Change image layout of one cubemap face to transfer destination
        Image::SetImageLayout(
            commandBuffers[i],
            depthImages[light]->GetImage(),
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            subresourceRange);

        // Copy region for transfer from framebuffer to cube face
        VkImageCopy copyRegion = {};

        copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        copyRegion.srcSubresource.baseArrayLayer = 0;
        copyRegion.srcSubresource.mipLevel = 0;
        copyRegion.srcSubresource.layerCount = 1;
        copyRegion.srcOffset = { 0, 0, 0 };

        copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        copyRegion.dstSubresource.baseArrayLayer = 0;
        copyRegion.dstSubresource.mipLevel = 0;
        copyRegion.dstSubresource.layerCount = 1;
        copyRegion.dstOffset = { 0, 0, 0 };

        copyRegion.extent.width = depthImages[light]->GetWidth();
        copyRegion.extent.height = depthImages[light]->GetHeight();
        copyRegion.extent.depth = 1;

        // Put image copy into command buffer
        vkCmdCopyImage(
            commandBuffers[i],
            depthRenderPass->GetDepthImage()->GetImage(),
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            depthImages[light]->GetImage(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &copyRegion);

        // Transform framebuffer color attachment back 
        Image::SetImageLayout(
            commandBuffers[i],
            depthRenderPass->GetDepthImage()->GetImage(),
            VK_IMAGE_ASPECT_DEPTH_BIT,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        // Change image layout of copied face to shader read
        Image::SetImageLayout(
            commandBuffers[i],
            depthImages[light]->GetImage(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            subresourceRange);
    }



    // The final pass to render
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

    VkViewport viewport2 = {};
    viewport2.x = 0.0f;
    viewport2.y = 0.0f;
    viewport2.width = (float)GetExtent().width;
    viewport2.height = (float)GetExtent().height;
    viewport2.minDepth = 0.0f;
    viewport2.maxDepth = 1.0f;

    VkRect2D scissor2 = {};
    scissor2.offset = { 0, 0 };
    scissor2.extent = GetExtent();


    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdSetViewport(commandBuffers[i], 0, 1, &viewport2);

    vkCmdSetScissor(commandBuffers[i], 0, 1, &scissor2);

    // std::cout << "THERE ARE: " << objects.size() << std::endl;
    for (auto obj : objects) {
        // ONLY FOR TEST
        obj->BindCommandsToBuffer(commandBuffers[i]);
    }

    vkCmdEndRenderPass(commandBuffers[i]);

    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}


void SingleThreadRenderer::CreateSwapChain() {

    VulkanInstance::SwapChainSupportDetails swapChainSupport = VulkanInstance::GetInstance().QuerySwapChainSupport(VulkanInstance::GetInstance().physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = VulkanInstance::ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = VulkanInstance::ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = VulkanInstance::ChooseSwapExtent(swapChainSupport.capabilities);

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
        throw std::runtime_error("RenderThread::CreateSwapChain: failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(VulkanInstance::GetInstance().device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(VulkanInstance::GetInstance().device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

void SingleThreadRenderer::CreateImageViews() {
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


void SingleThreadRenderer::CreateFramebuffers() {
    swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        std::array<VkImageView, 3> attachments = {
            renderPass->GetColorImage()->GetImageView(),
            renderPass->GetDepthImage()->GetImageView(),
            swapChainImageViews[i]
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

    // depth frame buffer
    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = depthRenderPass->GetRenderPass();
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &depthRenderPass->GetDepthImage()->GetImageView();
    framebufferInfo.width = depthRenderPass->GetDepthImage()->GetWidth();
    framebufferInfo.height = depthRenderPass->GetDepthImage()->GetHeight();
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(VulkanInstance::GetInstance().device, &framebufferInfo, nullptr, &depthBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
    }
}

void SingleThreadRenderer::CreateCommandPool() {
    VulkanInstance::QueueFamilyIndices queueFamilyIndices = VulkanInstance::GetInstance().FindQueueFamilies(VulkanInstance::GetInstance().physicalDevice);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(VulkanInstance::GetInstance().device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }
}

void SingleThreadRenderer::CreateCommandBuffers() {
    commandBuffers.resize(swapChainFramebuffers.size());

    updateCommands = std::vector<bool>(swapChainFramebuffers.size(), true);

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(VulkanInstance::GetInstance().device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void SingleThreadRenderer::CreateSyncObjects() {
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

void SingleThreadRenderer::CleanupSwapChain() {

    vkFreeCommandBuffers(VulkanInstance::GetInstance().device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(VulkanInstance::GetInstance().device, framebuffer, nullptr);
    }

    vkDestroyFramebuffer(VulkanInstance::GetInstance().device, depthBuffer, nullptr);


    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(VulkanInstance::GetInstance().device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(VulkanInstance::GetInstance().device, swapChain, nullptr);
}

void SingleThreadRenderer::RecreateSwapChain() {
    int width = 0, height = 0;

    glfwGetFramebufferSize(VulkanInstance::GetInstance().window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(VulkanInstance::GetInstance().window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(VulkanInstance::GetInstance().device);

    VulkanInstance::GetInstance().width = width;
    VulkanInstance::GetInstance().height = height;

    CleanupSwapChain();

    CreateSwapChain();

    renderPass->ResizeImages();

    CreateImageViews();
    CreateFramebuffers();
    CreateCommandBuffers();
}


void SingleThreadRenderer::CleanUp() {

    vkDeviceWaitIdle(VulkanInstance::GetInstance().device);

    CleanupSwapChain();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(VulkanInstance::GetInstance().device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(VulkanInstance::GetInstance().device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(VulkanInstance::GetInstance().device, inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(VulkanInstance::GetInstance().device, commandPool, nullptr);

    for (auto& entry : deleteInfoVector) {
        entry.func();
    }

    for (auto& entry : _uniforms) {
        delete entry.second;
    }
      
    vkDestroyDescriptorPool(VulkanInstance::GetInstance().device, _descriptorPool, nullptr);

    vkDestroySampler(VulkanInstance::GetInstance().device, depthSampler, nullptr);

    for (auto image : depthImages) {
        delete image;
    }

    delete _shadowMaterial;

    delete renderPass;
    delete depthRenderPass;
}

void SingleThreadRenderer::AddObject(Renderizable* obj) {
    objectsToAdd.push_back(obj);
}


void SingleThreadRenderer::RemoveObject(Renderizable* obj, std::function<void()> func) {

    objectsToRemove.push_back(obj);
    if (func != NULL) {
        deleteInfoVector.push_back({ obj, std::vector<bool>(swapChainFramebuffers.size(),false),func });
    }

}