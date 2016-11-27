#include <iostream>
#include <vector>
#include <cstring>

#include "TriVEngine.h"
#include <map>
#include <set>
#include <algorithm>
#include <fstream>


namespace TriV
{




	TriVEngine::TriVEngine()
	{
	}
	
	
	TriVEngine::~TriVEngine()
	{
	}
	
	void TriVEngine::initiateEngine()
	{
		std::cout << ">>>>>INITIATING TRIVENGINE<<<<<" << std::endl;
		initWindow();
		initVulkan();
		mainLoop();
	}
	
	void TriVEngine::initWindow()
	{
		std::cout << ">>>INITIATING GLFW<<<" << std::endl;
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		if (glfwVulkanSupported() == GLFW_FALSE)
		{
			throw std::runtime_error("GLFW ERROR: NO VULKAN LOADER FOUND");
		}
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	}

	void TriVEngine::initVulkan()
	{
		std::cout << ">>>INITIATING VULKAN<<<" << std::endl;
		createInstance();
		setupDebugCallback();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createGraphicsPipeline();
	}

	void TriVEngine::createSurface()
	{
		if(glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE WINDOW SURFACE!");
		}
	}

	void TriVEngine::mainLoop()
	{
		std::cout << ">>>INITIATING MAIN LOOP<<<" << std::endl;
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}
	
	void TriVEngine::createInstance()
	{

		if(enableValidationLayers && !checkValidationLayerSupport())
		{
			throw std::runtime_error("VALIDATION LAYERS REQUESTED, BUT NOT AVAILABLE!");
		}
	
		// Application Info
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = APPLICATION_TITLE;
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = ENGINE_TITLE;
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;
	
		// Instance Info
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		if(enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}else
		{
			createInfo.enabledLayerCount = 0;
		}
	
		// Vulkan Extensions
		uint32_t vkExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(vkExtensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, extensions.data());
		std::cout << "Available Vulkan Extensions: " << std::endl;
		for (const auto& extension : extensions)
		{
			std::cout << "\t" << extension.extensionName << std::endl;
		}
	
		// GLFW Extensions
		auto glfwExtensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
		createInfo.ppEnabledExtensionNames = glfwExtensions.data();
		createInfo.enabledLayerCount = 0;
		std::cout << "Required Vulkan Extensions: " << std::endl;
		for(const auto& extension : glfwExtensions)
		{
			std::cout << "\t" << extension << std::endl;
		}
	
		// Create the actual instance
		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE VULKAN INSTANCE!");
		}
		std::cout << "SUCCESSFULLY CREATED VULKAN INSTANCE" << std::endl;
	}

	void TriVEngine::pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if(deviceCount == 0)
		{
			throw std::runtime_error("FAILED TO FIND GPUS WITH VULKAN SUPPORT!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		// Use an ordered map to automatically sort candidates by increasing score
		std::map<uint64_t, VkPhysicalDevice> candidates;

		std::cout << "FOUND " << devices.size() << " GRAPHICS DEVICES" << std::endl;
		for (const auto& device : devices) {
			uint64_t score = rateDeviceSuitability(device);
			std::cout << "\t" << "SCORE: " << score << std::endl;
			candidates[score] = device;
		}

		// Check if the best candidate is suitable at all
		if (candidates.begin()->first > 0) {
			physicalDevice = candidates.begin()->second;
		}
		else {
			throw std::runtime_error("FAILED TO FIND A SUITABLE GPU!");
		}
	}

	uint64_t TriVEngine::rateDeviceSuitability(VkPhysicalDevice device) const
	{
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		std::cout << "RATING GRAPHICS DEVICE: " << deviceProperties.deviceName << std::endl;

		uint64_t score = 0;

		// Discrete GPUs have a significant performance advantage
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 1000;
		}

		// Maximum possible size of textures affects graphics quality
		score += deviceProperties.limits.maxImageDimension2D;
		score += deviceProperties.limits.maxMemoryAllocationCount;
		score += deviceProperties.limits.maxSamplerAllocationCount;

		QueueFamilyIndices indices = findQueueFamilies(device);

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if(extensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		if(!indices.isComplete() || !extensionsSupported || !swapChainAdequate)
		{
			std::cerr << "\t" << "DEVICE DOES NOT SUPPORT REQUIRED EXTENSIONS, QUEUES OR SWAP-CHAIN IS INADEQUATE!" << std::endl;
			return 0;
		}

		return score;
	}

	TriVEngine::QueueFamilyIndices TriVEngine::findQueueFamilies(VkPhysicalDevice device) const
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		size_t i = 0;
		for(const auto& queueFamily : queueFamilies)
		{
			if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = static_cast<int32_t>(i);
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, static_cast<uint32_t>(i), surface, &presentSupport);

			if(queueFamily.queueCount > 0 && presentSupport)
			{
				indices.presentFamily = static_cast<int32_t>(i);
			}

			if(indices.isComplete())
			{
				break;
			}

			++i;
		}

		return indices;
	}

	bool TriVEngine::checkDeviceExtensionSupport(VkPhysicalDevice device) const
	{
		std::cout << "\t" << "CHECKING FOR SUPPORTED EXTENSIONS" << std::endl;
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
		for(const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}
		return requiredExtensions.empty();
	}

	TriVEngine::SwapChainSupportDetails TriVEngine::querySwapChainSupport(VkPhysicalDevice device) const
	{
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
		if(formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if(presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR TriVEngine::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
	{
		if(availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
		{
			return{VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
		}

		for(const auto& availableFormat : availableFormats)
		{
			if(availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		//TODO: Rate formats based on score and return best one.

		return availableFormats[0];
	}

	VkPresentModeKHR TriVEngine::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const
	{
		for(const auto& availablePresentMode : availablePresentModes)
		{
			//TODO: Change to dynamic array of modes in preferred order.
			if(availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR || VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D TriVEngine::chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities) const
	{
		if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}

		VkExtent2D actualExtent = {WINDOW_WIDTH, WINDOW_HEIGHT};
		actualExtent.width =  std::max(capabilities.minImageExtent.width,  std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
		return actualExtent;
	}

	void TriVEngine::createImageViews()
	{
		swapChainImageViews.resize(swapChainImages.size(), VDeleter<VkImageView>{device, vkDestroyImageView});
		for(size_t i = 0; i < swapChainImages.size(); ++i)
		{
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

			if(vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("FAILED TO CREATE IMAGE VIEWS!");
			}
		}

		std::cout << "SUCCESSFULLY CREATED IMAGE VIEWS!" << std::endl;
	}

	void TriVEngine::createSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		//TODO: COMMENT OUT
		//createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
		uint32_t queueFamilyIndices[] = { static_cast<uint32_t>(indices.graphicsFamily), static_cast<uint32_t>(indices.presentFamily)};

		if(indices.graphicsFamily != indices.presentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE SWAP-CHAIN!");
		}

		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;

		std::cout << "SUCCESSFULLY CREATED SWAP-CHAIN" << std::endl;
	}

	void TriVEngine::createLogicalDevice()
	{
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<int32_t> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

		for(int32_t queueFamily : uniqueQueueFamilies)
		{
			float queuePriority = 1.0f;
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};
		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if(enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}else
		{
			createInfo.enabledLayerCount = 0;
		}

		if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE LOGICAL DEVICE!");
		}

		vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
		vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
	}

	void TriVEngine::createGraphicsPipeline() const
	{
		std::cout << "LOADING SHADERS:" << std::endl;
		auto vertexShaderCode = loadShader("Shaders/vert.spv");
		auto fragmentShaderCode = loadShader("Shaders/frag.spv");

		VDeleter<VkShaderModule> vertexShaderModule{device, vkDestroyShaderModule};
		VDeleter<VkShaderModule> fragmentShaderModule{ device, vkDestroyShaderModule };

		createShaderModule(vertexShaderCode, vertexShaderModule);
		createShaderModule(fragmentShaderCode, fragmentShaderModule);

		VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
		vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertexShaderStageInfo.module = vertexShaderModule;
		vertexShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
		fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragmentShaderStageInfo.module = fragmentShaderModule;
		fragmentShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = {vertexShaderStageInfo, fragmentShaderStageInfo};
	}

	std::vector<char> TriVEngine::loadShader(const std::string & shaderpath)
	{
		std::cout << "\t" << "LOADING SHADER: " << shaderpath.c_str() << std::endl;
		std::ifstream file(shaderpath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error(std::string("\tFAILED TO LOAD SHADER: ") + shaderpath);
		}
		size_t fileSize = file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();
		std::cout << "\t" << "SUCCESSFULLY LOADED SHADER: " << shaderpath.c_str() << std::endl;
		return buffer;
	}

	void TriVEngine::createShaderModule(const std::vector<char>& code, VDeleter<VkShaderModule>& shaderModule) const
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE SHADER MODULE!");
		}

		std::cout << "SUCCESSFULLY CREATED SHADER MODULE!" << std::endl;
	}

	bool TriVEngine::checkValidationLayerSupport() const
	{
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

	std::vector<const char*> TriVEngine::getRequiredExtensions() const
	{
		std::vector<const char*> extensions;
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (uint32_t i = 0; i < glfwExtensionCount; ++i) {
			extensions.push_back(glfwExtensions[i]);
		}

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		}

		return extensions;
	}

	void TriVEngine::setupDebugCallback()
	{
		if (!enableValidationLayers)
		{
			return;
		}

		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		createInfo.pfnCallback = reinterpret_cast<PFN_vkDebugReportCallbackEXT>(DebugCallback);

		auto CreateDebugReportCallback = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));

		if (!CreateDebugReportCallback || CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS) {
			throw std::runtime_error("FAILED TO SET UP DEBUG CALLBACK!");
		}

		std::cout << "SUCCESSFULLY SET UP DEBUG CALLBACK" << std::endl;
	}

	VkBool32 TriVEngine::DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
		std::cerr << "validation layer: " << msg << std::endl;
		return VK_FALSE;
	}

	VkResult TriVEngine::CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
		std::cout << "CREATING DEBUG REPORT CALLBACK!" << std::endl;
		auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void TriVEngine::DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks * pAllocator)
	{
		std::cout << "DESTROYING DEBUG REPORT CALLBACK!" << std::endl;
		auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
		if (func != nullptr) {
			func(instance, callback, pAllocator);
		}
	}
}
