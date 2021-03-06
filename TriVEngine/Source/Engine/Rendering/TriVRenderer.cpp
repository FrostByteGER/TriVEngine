#include <iostream>
#include <vector>

#include "TriVRenderer.hpp"
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <fstream>


namespace TriV
{


	TriVRenderer::TriVRenderer()
	{
	}
	
	
	TriVRenderer::~TriVRenderer()
	{
	}
	
	void TriVRenderer::initiateEngine()
	{
		std::cout << ">>>>>INITIATING TRIVENGINE<<<<<" << std::endl;
		initWindow();
		initVulkan();
		mainLoop();
	}
	
	void TriVRenderer::initWindow()
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

	void TriVRenderer::initVulkan()
	{
		std::cout << ">>>INITIATING VULKAN<<<" << std::endl;
		createInstance();
		setupDebugCallback();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandPool();
		createCommandBuffers();
		createSemaphores();
	}

	void TriVRenderer::createSurface()
	{
		if(glfwCreateWindowSurface(instance, window, nullptr, surface.Replace()) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE WINDOW SURFACE!");
		}
	}

	void TriVRenderer::mainLoop()
	{
		std::cout << ">>>INITIATING MAIN LOOP<<<" << std::endl;
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(device);
	}
	
	void TriVRenderer::createInstance()
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

		// GLFW Extensions
		auto glfwExtensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
		createInfo.ppEnabledExtensionNames = glfwExtensions.data();
		std::cout << "Required Vulkan Extensions: " << std::endl;
		for (const auto& extension : glfwExtensions)
		{
			std::cout << "\t" << extension << std::endl;
		}

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

		// Create the actual instance
		VkResult result = vkCreateInstance(&createInfo, nullptr, instance.Replace());
	
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE VULKAN INSTANCE!");
		}
		std::cout << "SUCCESSFULLY CREATED VULKAN INSTANCE" << std::endl;
	}

	void TriVRenderer::pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if(deviceCount == 0)
		{
			throw std::runtime_error("FAILED TO FIND GPUs WITH VULKAN SUPPORT!");
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

	uint64_t TriVRenderer::rateDeviceSuitability(VkPhysicalDevice device) const
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

	TriVRenderer::QueueFamilyIndices TriVRenderer::findQueueFamilies(VkPhysicalDevice device) const
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		std::size_t i = 0;
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

	bool TriVRenderer::checkDeviceExtensionSupport(VkPhysicalDevice device) const
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

	TriVRenderer::SwapChainSupportDetails TriVRenderer::querySwapChainSupport(VkPhysicalDevice device) const
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

	VkSurfaceFormatKHR TriVRenderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
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

	VkPresentModeKHR TriVRenderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const
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

	VkExtent2D TriVRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities) const
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

	void TriVRenderer::createImageViews()
	{
		swapChainImageViews.resize(swapChainImages.size(), VDeleter<VkImageView>{device, vkDestroyImageView});
		for(std::size_t i = 0; i < swapChainImages.size(); ++i)
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

			if(vkCreateImageView(device, &createInfo, nullptr, swapChainImageViews[i].Replace()) != VK_SUCCESS)
			{
				throw std::runtime_error("FAILED TO CREATE IMAGE VIEWS!");
			}
		}

		std::cout << "SUCCESSFULLY CREATED IMAGE VIEWS!" << std::endl;
	}

	void TriVRenderer::createSwapChain()
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
		createInfo.imageExtent = extent;
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
		createInfo.oldSwapchain = nullptr;

		if(vkCreateSwapchainKHR(device, &createInfo, nullptr, swapChain.Replace()) != VK_SUCCESS)
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

	void TriVRenderer::createLogicalDevice()
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

		if(vkCreateDevice(physicalDevice, &createInfo, nullptr, device.Replace()) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE LOGICAL DEVICE!");
		}

		vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
		vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
	}

	void TriVRenderer::createRenderPass()
	{
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subPass = {};
		subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subPass.colorAttachmentCount = 1;
		subPass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subPass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if(vkCreateRenderPass(device, &renderPassInfo, nullptr, renderPass.Replace()) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE RENDER PASS");
		}
		std::cout << "SUCCESSFULLY CREATED RENDER PASS" << std::endl;
	}

	void TriVRenderer::createGraphicsPipeline()
	{
		std::cout << "CREATING GRAPHICS-PIPELINE" << std::endl;
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

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapChainExtent.width);
		viewport.height = static_cast<float>(swapChainExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0;

		VkRect2D scissor = {};
		scissor.offset = { 0,0 };
		scissor.extent = swapChainExtent;

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;

		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, pipelineLayout.Replace()) != VK_SUCCESS) 
		{
			throw std::runtime_error("FAILED TO CREATE PIPELINE LAYOUT!");
		}

		std::cout << "SUCCESSFULLY CREATED PIPELINE-LAYOUT" << std::endl;

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = nullptr;
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = nullptr;
		pipelineInfo.basePipelineIndex = -1;

		if(vkCreateGraphicsPipelines(device, nullptr, 1, &pipelineInfo, nullptr, graphicsPipeline.Replace()) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE GRAPHICS-PIPELINE!");
		}

		std::cout << "SUCCESSFULLY CREATED GRAPHICS-PIPELINE" << std::endl;
	}

	void TriVRenderer::createFramebuffers()
	{
		swapChainFramebuffers.resize(swapChainImageViews.size(), VDeleter<VkFramebuffer>{device, vkDestroyFramebuffer});

		for(std::size_t i = 0; i < swapChainImageViews.size(); ++i)
		{
			VkImageView attachments[] = {
				swapChainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;

			if(vkCreateFramebuffer(device, &framebufferInfo, nullptr, swapChainFramebuffers[i].Replace()) != VK_SUCCESS)
			{
				throw std::runtime_error("FAILED TO CREATE FRAMEBUFFER!");
			}
		}

		std::cout << "SUCCESSFULLY CREATED FRAMEBUFFERS" << std::endl;
	}

	void TriVRenderer::createCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		poolInfo.flags = 0;

		if(vkCreateCommandPool(device, &poolInfo, nullptr, commandPool.Replace()) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE COMMANDPOOL!");
		}

		std::cout << "SUCCESSFULLY CREATED COMMANDPOOL" << std::endl;
	}

	void TriVRenderer::createCommandBuffers()
	{
		commandBuffers.resize(swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if(vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO ALLOCATE COMMANDBUFFERS!");
		}

		std::cout << "SUCCESSFULLY ALLOCATED COMMANDBUFFERS" << std::endl;

		for(std::size_t i = 0; i < commandBuffers.size(); ++i)
		{
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			beginInfo.pInheritanceInfo = nullptr;

			vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass;
			renderPassInfo.framebuffer = swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = swapChainExtent;
			
			VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			// Begin RenderPass
			{
				vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
				vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
				vkCmdEndRenderPass(commandBuffers[i]);
			}
			// End RenderPass

			if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("FAILED TO RECORD COMMANDBUFFER!");
			}
		}

		std::cout << "SUCCESSFULLY RECORDED COMMANDBUFFERS" << std::endl;
	}

	void TriVRenderer::createSemaphores()
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if(vkCreateSemaphore(device, &semaphoreInfo, nullptr, imageAvailableSemaphore.Replace()) != VK_SUCCESS ||    vkCreateSemaphore(device, &semaphoreInfo, nullptr, renderFinishedSemaphore.Replace()) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE SEMAPHORES!");
		}
	}

	void TriVRenderer::drawFrame()
	{
		uint32_t imageIndex;
		vkAcquireNextImageKHR(device, swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, nullptr, &imageIndex);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if(vkQueueSubmit(graphicsQueue, 1, &submitInfo, nullptr) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO SUBMIT DRAW COMMANDBUFFER!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(presentQueue, &presentInfo);

	}

	std::vector<char> TriVRenderer::loadShader(const std::string & shaderpath)
	{
		std::cout << "\t" << "LOADING SHADER: " << shaderpath.c_str() << std::endl;
		std::ifstream file(shaderpath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error(std::string("\tFAILED TO LOAD SHADER: ") + shaderpath);
		}
		std::size_t fileSize = file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();
		std::cout << "\t" << "SUCCESSFULLY LOADED SHADER: " << shaderpath.c_str() << std::endl;
		return buffer;
	}

	void TriVRenderer::createShaderModule(const std::vector<char>& code, VDeleter<VkShaderModule>& shaderModule) const
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if(vkCreateShaderModule(device, &createInfo, nullptr, shaderModule.Replace()) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE SHADER MODULE!");
		}

		std::cout << "SUCCESSFULLY CREATED SHADER MODULE!" << std::endl;
	}

	bool TriVRenderer::checkValidationLayerSupport() const
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (std::strcmp(layerName, layerProperties.layerName) == 0) {
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

	std::vector<const char*> TriVRenderer::getRequiredExtensions() const
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

	void TriVRenderer::setupDebugCallback()
	{
		if (!enableValidationLayers)
		{
			return;
		}

		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		createInfo.pfnCallback = DebugCallback;

		if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, callback.Replace()) != VK_SUCCESS) {
			throw std::runtime_error("FAILED TO SET UP DEBUG CALLBACK!");
		}

		std::cout << "SUCCESSFULLY SET UP DEBUG CALLBACK" << std::endl;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL TriVRenderer::DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, std::size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
		std::cerr << "ERROR: Validation layer: " << msg << std::endl;

		return VK_FALSE;
	}

	VkResult TriVRenderer::CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
		std::cout << "CREATING DEBUG REPORT CALLBACK!" << std::endl;
		auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void TriVRenderer::DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
		std::cout << "DESTROYING DEBUG REPORT CALLBACK!" << std::endl;
		auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
		if (func != nullptr) {
			func(instance, callback, pAllocator);
		}
	}
}
