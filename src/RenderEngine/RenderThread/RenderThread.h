#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <thread>
#include <vector>
#include <atomic>

class VulkanInstance;
class Image;
class RenderPass;
class Renderizable;

/*
	Singleton to represent the render thread
*/
class RenderThread
{
private:

	bool _initialized = false;

	bool _running = false;

	std::thread* _mainThread = nullptr;

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

	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	RenderPass* renderPass;

	std::vector<Renderizable*> objects;

	std::vector<Renderizable*> objectsToAdd;

	static RenderThread* _instance;

	RenderThread();

	void MainLoop();

	void DrawFrame();

	void CreateSwapChain();

	void CreateImageViews();

	void CreateFramebuffers();

	void CreateCommandPool();

	void CreateCommandBuffers();

	void CreateSyncObjects();

	void CleanupSwapChain();

	void RecreateSwapChain();

public:
	const int MAX_FRAMES_IN_FLIGHT = 5;

	~RenderThread();

	/* Get the singleton */
	static RenderThread& GetInstance();

	/* Initializate swapchain, needed to call before using GetFormat, GetExtent*/
	void InitializeSwapChain();

	/* Initialize the window, vulkan and other things */
	void Initialize(RenderPass* renderPass);

	/* Start the render thread */
	void StartThread();

	/* Stop the render thread */
	void StopThread();

	/* Re record the command buffers orders with the list */
	void UpdateCommandBuffers();

	/* Add renderizable to the render */
	void AddObject(Renderizable* obj);

	/* CleanUp all thre resources generated */
	void CleanUp();
	
	VkFormat GetFormat() const {
		return swapChainImageFormat;
	}

	VkExtent2D GetExtent() const {
		return swapChainExtent;
	}

	std::atomic<int> frames;
};

