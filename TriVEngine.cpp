#include <iostream>
#include <vector>

#include "TriVEngine.h"


namespace TriV
{
	TriVEngine::TriVEngine()
	{
	}
	
	
	TriVEngine::~TriVEngine()
	{
	}
	
	void TriVEngine::run()
	{
		initWindow();
		initVulkan();
		mainLoop();
	}
	
	void TriVEngine::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		if (glfwVulkanSupported() == GLFW_FALSE)
		{
			throw std::runtime_error("GLFW ERROR: NO VULKAN LOADER FOUND");
		}
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	}
	
	void TriVEngine::createInstance()
	{
	
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
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;
	
		// Create the actual instance
		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE VULKAN INSTANCE!");
		}
		std::cout << "Successfully created Vulkan Instance";
	}
	
	void TriVEngine::initVulkan()
	{
		createInstance();
	}
	
	void TriVEngine::mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}
}
