#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>

/*
	Class to store the vulkan instance, window, surface, the devices and the queues asociated to that devices

	IMPROVEMENTS: Separate window and surface from this class to another representation
				  Allow multiple phyisical devices
*/

class VulkanInstance
{
public:
	//Pointer to the window where the app runs
	GLFWwindow* window;
	int width = 1080, height = 720;


	//Vulkan instance
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;

	//Physical and logical devices
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	//Queues of the device (graphics family and present famlily)
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	bool framebufferResized = false;

	//Default validation layers
	std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	//Default extensions used
	std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	/*
		Only use validation layers on debug mode
	*/
	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else
		const bool enableValidationLayers = true;
	#endif

	/*
		Used to know if a device has the graphics and present family queues
		as its not a requeriment but an optimal feature, use the optional to put a value or dont
	*/
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	/*
		Struct to store the deatils of the swap chain
	*/
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

public: 
	//Default constructor
	VulkanInstance();

	//Create a instance with the passed restrictions
	VulkanInstance(int width, int height, const std::vector<const char*>& validationLayers, const std::vector<const char*>& deviceExtensions);

	/*
		Initialize de instance, setup de debug messenger,  create a surface, pick the physical device and create the logical one
	*/
	void Initialize();

	/*
		Cleanup and destroy all the resources used
	*/
	void CleanUp();

public:
	/*
		Initialize the GLFW window where the app will run
	*/
	void CreateWindow(int WIDTH, int HEIGHT);

	/*
		Callback to call when the window is resized
	*/
	static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);

	/*
		Create the VKInstance using validationLayers and deviceExtensions
	*/
	void CreateInstance();

	/*
		Setup the use of validationLayers and the message that will print
	*/
	void SetupDebugMessenger();

	/*
		Create a surface
	*/
	void CreateSurface();

	/*
		Pick a physical device that follow the specifications needed

		IMPROVE to allow multiple physical device uses
	*/
	void PickPhysicalDevice();

	/*
		Create a logical device to handle the phyisical device and the Queue used by the app
	*/
	void CreateLogicalDevice();

	/*
		return the extensions required plus the debug extension if needed
	*/
	std::vector<const char*> GetRequiredExtensions();

	/*
		return true if validation layers are supported and available
	*/
	bool CheckValidationLayerSupport();

	/*
		Fill the struct createInfo with the parameters needed to create the debugMessenger, passing the function DebugCallback()
	*/
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	//debug callback for the validation layers. It simply print the message
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	/*
		return true if the device has all the resources to run the app
	*/
	bool IsDeviceSuitable(VkPhysicalDevice device);

	/*
		Get the details of the swapchains supported by a device
	*/
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	/*
		Check if device support the extensions in deviceExtensions
	*/
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	/*
		Look for the queues families that the device support.
		queues are used to submit commands to that queue, and there 
		are specific queues for each operation. for ex: compute commands, memory transfer,
		render commands, etc.
	*/
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	/*
		Debug utils for the validation layers
	*/
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger);

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, 
		VkDebugUtilsMessengerEXT debugMessenger, 
		const VkAllocationCallbacks* pAllocator);
};
