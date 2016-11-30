#pragma once

#define GLFW_INCLUDE_VULKAN


#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>

#include "VDeleter.h"


namespace TriV
{
	class TriVEngine
	{
		struct QueueFamilyIndices
		{
			int32_t graphicsFamily = -1;
			int32_t presentFamily  = -1;

			bool isComplete() const
			{
				return graphicsFamily >= 0 && presentFamily >= 0;
			}
		};

		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

	public:
		TriVEngine();
		~TriVEngine();

		void initiateEngine();

	private:

		// Static Methods
		static std::vector<char> loadShader(const std::string& shaderpath);
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, std::size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);
		static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
		static void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

		// GLFW/Vulkan Variables
		GLFWwindow* window = nullptr;
		VDeleter<VkInstance> instance{ vkDestroyInstance };
		VDeleter<VkDebugReportCallbackEXT> callback{ instance, DestroyDebugReportCallbackEXT };
		VDeleter<VkSurfaceKHR> surface{ instance, vkDestroySurfaceKHR };
		VDeleter<VkDevice> device{vkDestroyDevice};

		VDeleter<VkSwapchainKHR> swapChain{ device, vkDestroySwapchainKHR };
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VDeleter<VkImageView>> swapChainImageViews;
		
		VkPhysicalDevice physicalDevice = nullptr;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		VDeleter<VkRenderPass> renderPass{device, vkDestroyRenderPass};
		VDeleter<VkPipelineLayout> pipelineLayout{ device, vkDestroyPipelineLayout };
		VDeleter<VkPipeline> graphicsPipeline{ device, vkDestroyPipeline };

		// Engine Information
		const char* WINDOW_TITLE = "TriVEngine";
		const char* APPLICATION_TITLE = "TriVEngine";
		const char* ENGINE_TITLE = "TriVEngine";
		const uint32_t WINDOW_WIDTH = 1280;
		const uint32_t WINDOW_HEIGHT = 720;

		// Methods
		void initWindow();
		void createInstance();
		void pickPhysicalDevice();
		uint64_t rateDeviceSuitability(VkPhysicalDevice device) const;
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
		bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
		void createImageViews();
		void createSwapChain();
		void createLogicalDevice();
		void createRenderPass();
		void createGraphicsPipeline();
		void createShaderModule(const std::vector<char>& code, VDeleter<VkShaderModule>& shaderModule) const;
		void initVulkan();
		void mainLoop();
		bool checkValidationLayerSupport() const;
		std::vector<const char*> getRequiredExtensions() const;
		void setupDebugCallback();
		void createSurface();

		// Constants
		const std::vector<const char*> validationLayers = {
			"VK_LAYER_LUNARG_standard_validation"
		};
		const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif
	};
}
