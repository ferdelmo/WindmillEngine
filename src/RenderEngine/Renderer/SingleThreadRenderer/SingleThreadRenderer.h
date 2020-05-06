#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <functional>

#include "RenderEngine/Renderer/Material.h"

class VulkanInstance;
class Image;
class RenderPass;
class Renderizable;
class MaterialInstance;

/*
	Renderer to use in a single thread program, does not use mutex or other sync objects
*/
class SingleThreadRenderer
{

public:
	bool _initialized = false;

	/*
		Swapchain an the other things needed, that include the frame buffer and the image views
	*/
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkCommandPool commandPool;

	VkFramebuffer depthBuffer;

	struct DeleteInfo {
		Renderizable* obj;
		std::vector<bool> check;
		std::function<void()> func;
	};



	std::vector<bool> updateCommands;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	RenderPass* renderPass;
	RenderPass* depthRenderPass;

	std::vector<Renderizable*> objects;

	std::vector<Renderizable*> objectsToAdd;

	std::vector<Renderizable*> objectsToRemove;

	std::vector<DeleteInfo> deleteInfoVector;


	std::vector<Image*> depthImages;

	// USE FOR DEPTH INFO, RETHINK THE DECLARATION
	VkDescriptorPool _descriptorPool = VK_NULL_HANDLE;
	VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;

	MapUniforms _uniforms;
	MaterialInstance* _shadowMaterial;

	static SingleThreadRenderer* _instance;

	SingleThreadRenderer();

	void CreateSwapChain();

	void CreateImageViews();

	void CreateFramebuffers();

	void CreateCommandPool();

	void CreateCommandBuffers();

	void CreateSyncObjects();

	void CleanupSwapChain();

	void RecreateSwapChain();
	

public:
	VkSampler depthSampler;

	const int MAX_FRAMES_IN_FLIGHT = 5;

	static SingleThreadRenderer& GetInstance();

	~SingleThreadRenderer();

	/* Get the singleton */

	/* Initializate swapchain, needed to call before using GetFormat, GetExtent*/
	void InitializeSwapChain();

	/* Initialize the window, vulkan and other things */
	void Initialize();

	/* Draw a Frame, this should be call in the main loop */
	void DrawFrame();

	/* Re record the command buffers orders with the list */
	void UpdateCommandBuffer(size_t i);

	/* Add renderizable to the render */
	void AddObject(Renderizable* obj);

	/* Add renderizable to the render */
	void RemoveObject(Renderizable* obj, std::function<void()> func = NULL);

	/* CleanUp all thre resources generated */
	void CleanUp();

	VkFormat GetFormat() const {
		return swapChainImageFormat;
	}

	VkExtent2D GetExtent() const {
		return swapChainExtent;
	}

	RenderPass* GetRenderPass() const {
		return renderPass;
	}

	RenderPass* GetDepthRenderPass() const {
		return depthRenderPass;
	}

	std::atomic<int> frames;
};
