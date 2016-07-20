#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "VDeleter.h"


namespace TriV
{
	class TriVEngine
	{
	public:
		TriVEngine();
		~TriVEngine();

		void run();

	private:
		GLFWwindow* window = nullptr;
		VDeleter<VkInstance> instance{ vkDestroyInstance };
		const uint32_t WINDOW_WIDTH = 1280;
		const uint32_t WINDOW_HEIGHT = 720;
		const char* WINDOW_TITLE = "TriVEngine";
		const char* APPLICATION_TITLE = "TriVEngine";
		const char* ENGINE_TITLE = "TriVEngine";

		void initWindow();
		void createInstance();
		void initVulkan();
		void mainLoop();
	};
}
