#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <iostream>
#include <vector>
#include <optional>
#include <set>
#include <string>
#include <algorithm>
#include <fstream>

/*
	Only use validation layers on debug mode
*/
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class RenderInstance
{
private:
	//resolution of the window
	int WIDTH = 800;
	int HEIGHT = 600;

	//max frames in flight
	int MAX_FRAMES_IN_FLIGHT = 2;

	//Pointer to the window where the app runs
	GLFWwindow* _window;

	//Vulkan instance
	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debugMessenger;
	VkSurfaceKHR _surface;

	//Physical and logical devices
	VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
	VkDevice _device;

	//Queues of the device (graphics family and present famlily)
	VkQueue _graphicsQueue;
	VkQueue _presentQueue;

	//Swapchain -> special framebuffer. Infraestructure that will own the buffer to render before
	//visualization. Its objetive is to synchronize the presentation of images with the refresh rate
	//of the monitor
	VkSwapchainKHR _swapChain;
	//images in the swapchain
	//images in the swapchain
	std::vector<VkImage> _swapChainImages;
	//format and resolution of the swapchain images
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;

	//Imageview describes how to acces the image and wich part to access
	std::vector<VkImageView> _swapChainImageViews;

	//Render pass, pipeline layout and the final graphic pipeline
	VkRenderPass _renderPass;
	VkDescriptorSetLayout _descriptorSetLayout;
	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;

	//Frame buffers
	std::vector<VkFramebuffer> _swapChainFramebuffers;

	//Command pools and buffer
	VkCommandPool _commandPool;
	std::vector<VkCommandBuffer> _commandBuffers;

	//Semaphores to sync images
	std::vector<VkSemaphore> _imageAvailableSemaphores;
	std::vector<VkSemaphore> _renderFinishedSemaphores;
	std::vector<VkFence> _inFlightFences;
	std::vector<VkFence> _imagesInFlight;
	size_t _currentFrame = 0;

	bool _framebufferResized = false;

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	/*
		Struct with the deatils of the swap chain
	*/
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	/*
		Validation layers used
	*/
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	/*
		Extensions used in the app
	*/
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

public:

	RenderInstance();

	/*
		Initialize the Render Instance
	*/
	void Initialize();

	/*
		Clean all the resources generated
	*/
	void CleanUp();

	/*
		CleanUp the swapchain
	*/
	void CleanUpSwapChain();

	/*
		Recreate the swapchain
	*/
	void RecreateSwapChain();

	/*
		Create the Vulkan Instance
	*/
	void createInstance();

	/*
		Setup the debug messenger for the validation layers
	*/
	void setupDebugMessenger();

	/*
		Create the surface to render
	*/
	void createSurface();

	/*
		Pick the physical device to use in the instance

		IMPROVEMENT, potencial to use various
	*/
	void pickPhysicalDevice();

	/*
		Create the logical device to manage the device
	*/
	void createLogicalDevice();

	/*
		Create the swapchain where the images will be rendered
	*/
	void createSwapChain();

	/*
		Create the image views of the swap chain
	*/
	void createImageViews();

	/*
		Create the render pass

		IMPROVEMENT handle various render pass
	*/
	void createRenderPass();

	/*
		Create a description set layout
	*/
	void createDescriptorSetLayout();

	/*
		Create the graphics pipeline to use

		IMPROVEMENT handle varous pipelines
	*/
	void createGraphicsPipeline();

	/*
		Create frame Buffers
	*/
	void createFramebuffers();

	/*
		Create the command pool where the drawing commands will be submited
	*/
	void createCommandPool();
	
	/*
		Create the command buffers used to render the images
	*/
	void createCommandBuffers();

	/*
		Create the objects to sync the rendering with various frames in flight
	*/
	void createSyncObjects();


private:
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	/*
		return true if validation layers are supported and available
	*/
	bool checkValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	/*
		return the extensions required plus the debug extension if needed
	*/
	std::vector<const char*> getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		//Extensions required by GLFW
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		//Extension for the validation layer
		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	//debug callback for the validation layers. It simply print the message
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	/*
		return the details of the swap chain supported by the device
	*/
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	//return true if device have all the resource to run the app
	bool isDeviceSuitable(VkPhysicalDevice device) {
		//Check if has the queue families needed
		QueueFamilyIndices indices = findQueueFamilies(device);

		//check the extensions
		bool extensionsSupported = checkDeviceExtensionSupport(device);

		//check for swapceain support
		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
	}

	bool isDeviceSuitable(VkPhysicalDevice device) {
		//Check if has the queue families needed
		QueueFamilyIndices indices = findQueueFamilies(device);

		//check the extensions
		bool extensionsSupported = checkDeviceExtensionSupport(device);

		//check for swapceain support
		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
	}

	/*
		Check if the device support the extensions needed (only swapchain)
	*/
	bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	/*
		Look for the queues families that the device support.
		queues are used to submit commands to that queue, and there
		are specific queues for each operation. for ex: compute commands, memory transfer,
		render commands, etc.
	*/
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}

	/*
		Choose the format of the surface, check for SRGB with 32 bit per pixel (8 per channel)
	*/
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	/*
		Choose present mode. There are different modes (documentation)
		If the queue is full, instead of blocking, replace the old images with the newer ones
	*/
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	/*
		Choose the resolution to match the window resolution in GLFW if posible
		check if its in range of swapchain
	*/
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		if (capabilities.currentExtent.width != UINT32_MAX) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(_window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}
	
	/*
		Read a binary file as a vector of characters, used to read the shaders
	*/
	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	/*
		Create a shader module from a vector<char> that contains the shader bits
	*/
	VkShaderModule createShaderModule(const std::vector<char>& code) {
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

		//Pass the bits
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}

		return shaderModule;
	}
};

