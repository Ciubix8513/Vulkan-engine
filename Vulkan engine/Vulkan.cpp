#include "Vulkan.h"

Vulkan::Vulkan()
{
	m_instance = 0;
	m_messenger = 0;
	m_physicalDevice = 0;
	m_device = 0;
	m_graphicsQueue = 0;
	m_surface = 0;
	m_presentQueue = 0;
	m_swapChain = 0;
	m_pipelineLayout = 0;
	m_renderPass = 0;
	m_graphicsPipeline = 0;
	m_commandPool = 0;
	m_currentFrame = 0;
	m_resized = false;
}

void Vulkan::Init(GLFWwindow* wnd, Settings settings)
{
	
	//Applying settings
	m_settings = settings;
	//Set window ptr
	m_pWnd = wnd;
	//Create instance
	CreateInstance();
	//Setup debug messenger
	SetupDebugMessenger();
	//Create surface
	CreateSurface();
	//Pick the graphics card
	PickPhysicalDevice();
	//Create logical device
	CreateLogicalDevice();
	//Create swap chain
	CreateSwapChain();	
	//Create image views for swap chain buffers
	CreateImageViews();
	//Create render pass
	CreateRenderPass();
	//Create pipeline
	CreateGraphicsPipeline();
	//Create frame buffers
	CreateFrameBuffers();
	//Create command pool
	CreateCommandPools();
	//Create vertex buffer
	CreateVertexBuffer();
	//Create command buffer for every frame buffer
	CreateCommandBuffers();
	//Create semaphores
	CreateSyncObjects();
	

}

void Vulkan::CleanupSwapChain()
{
	//Destroying frame buffers
	for (size_t i = 0; i < m_swapChainFrameBuffers.size(); i++)
		vkDestroyFramebuffer(m_device, m_swapChainFrameBuffers[i], 0);
	vkFreeCommandBuffers(m_device, m_commandPool, m_commandBuffers.size(), m_commandBuffers.data());
	//Destroy pipeline
	vkDestroyPipeline(m_device, m_graphicsPipeline, 0);
	//Destroy render pass
	vkDestroyRenderPass(m_device, m_renderPass, 0);
	//Destroy pipeline layout
	vkDestroyPipelineLayout(m_device, m_pipelineLayout, 0);
	//Destroying image views
	for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
		vkDestroyImageView(m_device, m_swapChainImageViews[i], 0);
	//Destroy swap chain
	vkDestroySwapchainKHR(m_device, m_swapChain, 0);
}


void Vulkan::Shutdown()
{
	//Wait for gpu to finish
	vkDeviceWaitIdle(m_device);
	//Destroy vertex buffer
	vkDestroyBuffer(m_device, m_vertexBuffer, 0);
	//Free vertex buffer memory
	vkFreeMemory(m_device, m_vertexBufferMemory, 0);
	//Cleanup all swap chain related stuff
	CleanupSwapChain();
	//Destrtoying command pools
	vkDestroyCommandPool(m_device, m_commandPool, 0);
	vkDestroyCommandPool(m_device, m_commandPool1, 0);
	//Destroy sync objects
	for (size_t i = 0; i < m_settings.maxFramesInFlight; i++)
	{
		vkDestroySemaphore(m_device, m_imageAvailableSemaphores[i], 0);
		vkDestroySemaphore(m_device, m_renderFinishedSemaphores[i], 0);
		vkDestroyFence(m_device, m_inFlightFences[i], 0);
	}		
	//Destroy logical device
	vkDestroyDevice(m_device, 0);
	//Destroy messenger if debugging is enabled
	if (enableValidationLayers)
		DestroyDebugUtilsMessengerEXT(m_instance, m_messenger, 0);
	//Destroy surface
	vkDestroySurfaceKHR(m_instance, m_surface, 0);
	//Destroy instance
	vkDestroyInstance(m_instance, 0);
	return;
}

void Vulkan::CreateInstance()
{
	//Chech if validation layers are avalable
	if (enableValidationLayers && !CheckValidationLevelSupport())	
		throw std::runtime_error("Requested validation layers are not available");
	

	//Create a struct with app info
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "This is the engine lol";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();

	createInfo.enabledExtensionCount = extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();


	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
		//Fill debugCreate info
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = (PFN_vkDebugUtilsMessengerCallbackEXT)debugCallback;
		debugCreateInfo.pUserData = 0;
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = 0;
	}
	//Create instance
	if (vkCreateInstance(&createInfo, 0, &m_instance) != VK_SUCCESS)
		throw std::runtime_error("Failed to create instance");
	return;
}

bool Vulkan::CheckValidationLevelSupport()
{
	//Get number of available layers
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, 0);

	//Get Layers
	VkLayerProperties* availableLayers = new VkLayerProperties[layerCount];
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);


	for (size_t i = 0; i < validationLayers.size(); i++)
	{
		bool found = false;
		for (size_t j = 0; j < layerCount; j++)
		{
			if (strcmp(validationLayers[i], availableLayers[j].layerName))
			{
				found = true;
				break;
			}
			if (!found)
				return false;
		}
	}
	return true;
}

bool Vulkan::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, 0, &extensionCount, 0);
	
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, 0, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (size_t i = 0; i < extensionCount; i++)	
		requiredExtensions.erase(availableExtensions[i].extensionName);

	return requiredExtensions.empty();
}

void Vulkan::SetupDebugMessenger()
{
	//Do nothing if debugging is not enabled
	if (!enableValidationLayers)return;

	//Info needed for creation
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = (PFN_vkDebugUtilsMessengerCallbackEXT)debugCallback;
	createInfo.pUserData = 0;
	//Creating debug messenger
	if (CreateDebugUtilsMessengerExt(m_instance, &createInfo, 0, &m_messenger) != VK_SUCCESS)
		throw std::runtime_error("Failed to set up debug messenger");
	return;
}

void Vulkan::CreateSurface()
{
	if (glfwCreateWindowSurface(m_instance, m_pWnd, 0, &m_surface) != VK_SUCCESS)
		throw std::runtime_error("Could not create surface");
	return;
}

void Vulkan::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	//Get number of available physical devices
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, 0);
	if (deviceCount == 1)
		std::cerr << "Found " << deviceCount << " physical device\n";
	else if (deviceCount != 0)
		std::cerr << "Found " << deviceCount << " physical devices\n";
	else
	{
		throw std::runtime_error("No available physical devices found");
		return;
	}
	//Get all devices
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
	for (size_t i = 0; i < deviceCount; i++)
		if (isDeviceSuitable(devices[i]))
		{
			m_physicalDevice = devices[i];
			break;
		}


	if (m_physicalDevice == 0)
		throw std::runtime_error("Could not find a suitable device");

	return;

}

bool Vulkan::isDeviceSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(device, &features);

	uint32_t qFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &qFamilyCount, 0);
	std::vector<VkQueueFamilyProperties> Families(qFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &qFamilyCount, Families.data());

	std::cerr << "Device: " << deviceProperties.deviceName << " Driver version: " << deviceProperties.driverVersion << '\n';

	bool extensionsSupported = CheckDeviceExtensionSupport(device);
	//Getting queue families
	QueueFamilyIndecies indecies = findQueueFamilies(device);
	//Checking swapchain
	bool SwapChainAdequate = false;
	if(extensionsSupported)
	{
		SwapChainSupportDetails details = querySwapChainDetails(device);
		SwapChainAdequate = !details.formats.empty() && !details.modes.empty();
	}
	//TODO: add device selection



	return deviceProperties.deviceType == 2 && indecies.graphicsFamily.has_value() && indecies.presentFamily.has_value() && extensionsSupported && SwapChainAdequate;
}

QueueFamilyIndecies Vulkan::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndecies indecies;
	//Get queue families
	uint32_t qFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &qFamilyCount, 0);
	std::vector<VkQueueFamilyProperties> Families(qFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &qFamilyCount, Families.data());

	for (size_t i = 0; i < qFamilyCount; i++)
	{
		VkBool32 presentSupport = 0;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);
		if (presentSupport)
			indecies.presentFamily = i;
		if (Families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indecies.graphicsFamily = i;
	}

	return indecies;
}

SwapChainSupportDetails Vulkan::querySwapChainDetails(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;
	//Getting surface capabilities
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);
	//Getting formats
	uint32_t  formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, 0);
	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
	}
	//Getting modes
	uint32_t modeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &modeCount, 0);
	if(modeCount != 0)
	{
		details.modes.resize(modeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &modeCount, details.modes.data());
	}
	return details;
}

void Vulkan::CreateLogicalDevice()
{
	//Queue familiy creation info
	QueueFamilyIndecies indecies = findQueueFamilies(m_physicalDevice);
	std::vector<VkDeviceQueueCreateInfo> qCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indecies.graphicsFamily.value(),indecies.presentFamily.value() };
	float queuePriority = 1.0f;

	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo qCreateInfo{};
		qCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		qCreateInfo.queueFamilyIndex = queueFamily;
		qCreateInfo.queueCount = 1;
		qCreateInfo.pQueuePriorities = &queuePriority;
		qCreateInfos.push_back(qCreateInfo);
	}

	//Specifiying used features 
	VkPhysicalDeviceFeatures features{}; //empty for now
	//Device info
	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.queueCreateInfoCount = qCreateInfos.size();
	deviceCreateInfo.pQueueCreateInfos = qCreateInfos.data();
	deviceCreateInfo.pEnabledFeatures = &features;
	//Specify device extensions
	deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	//Specifying validation layers
	if (enableValidationLayers)
	{
		deviceCreateInfo.enabledLayerCount = validationLayers.size();
		deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
		deviceCreateInfo.enabledLayerCount = 0;

	//Creating the device
	if (vkCreateDevice(m_physicalDevice, &deviceCreateInfo, 0, &m_device) != VK_SUCCESS)
		throw std::runtime_error("Could not create logical device");
	//Get graphics queue handle
	vkGetDeviceQueue(m_device, indecies.graphicsFamily.value(), 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, indecies.presentFamily.value(), 0, &m_presentQueue);
	return;

}

void Vulkan::CreateSwapChain()
{
	//Getting swapchain support details
	SwapChainSupportDetails details = querySwapChainDetails(m_physicalDevice);
	//Getting all nescesary data
	auto format = ChooseFormat(details.formats);
	auto mode = ChoosePresentMode(details.modes);
	auto extent = ChooseExtent(details.capabilities);
	uint32_t imageCount = std::clamp(m_settings.swapChainImgCount,details.capabilities.minImageCount,details.capabilities.maxImageCount);
	QueueFamilyIndecies indecies = findQueueFamilies(m_physicalDevice);
	uint32_t qFamilyIndecies[] = { indecies.graphicsFamily.value(),indecies.presentFamily.value() };
	//Creation info
	VkSwapchainCreateInfoKHR info{};
	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.surface = m_surface;	
	info.minImageCount = imageCount;
	info.imageFormat = format.format;
	info.imageColorSpace = format.colorSpace;
	info.imageExtent = extent;
	info.imageArrayLayers = 1;
	info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //For now, replace later when add post proccesing
	if (indecies.graphicsFamily != indecies.presentFamily)
	{
		info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		info.queueFamilyIndexCount = 2;
		info.pQueueFamilyIndices = qFamilyIndecies;
	}
	else
	{
		info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.queueFamilyIndexCount = 0;
		info.pQueueFamilyIndices = 0;
	}
	info.preTransform = details.capabilities.currentTransform;
	info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	info.presentMode = mode;
	info.clipped = VK_TRUE;
	info.oldSwapchain = VK_NULL_HANDLE;
	
	//Creating swap chain
	VkResult res = vkCreateSwapchainKHR(m_device, &info, 0, &m_swapChain);
	if (res != VK_SUCCESS)
		throw std::runtime_error("Could not create swap chain");

	vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, 0);
	m_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, m_swapChainImages.data());
	
	m_swapChainFormat = format.format;
	m_swapChainExtent = extent;

	return;
}

VkSurfaceFormatKHR Vulkan::ChooseFormat(const std::vector<VkSurfaceFormatKHR>& formats)
{
	for (size_t i = 0; i < formats.size(); i++)
		if (formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && formats[i].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
			return formats[i];
	return formats[0];
}

VkPresentModeKHR Vulkan::ChoosePresentMode(const std::vector<VkPresentModeKHR>& modes)
{
	for (size_t i = 0; i < modes.size(); i++)
		if (modes[i] == m_settings.presentMode)
			return modes[i];
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Vulkan::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX)
		return capabilities.currentExtent;
	else
	{
		int width, height;
		glfwGetFramebufferSize(m_pWnd, &width, &height);
		VkExtent2D actualExtent = { (uint32_t)width,(uint32_t)height };
		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		return actualExtent;
	}
	
}

void Vulkan::CreateImageViews()
{
	m_swapChainImageViews.resize(m_swapChainImages.size());
	for (size_t i = 0; i < m_swapChainImages.size(); i++)
	{
		VkImageViewCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.image = m_swapChainImages[i];
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		info.format = m_swapChainFormat;
		info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		info.subresourceRange.baseArrayLayer = 0;
		info.subresourceRange.baseMipLevel = 0;
		info.subresourceRange.layerCount = 1;
		info.subresourceRange.levelCount = 1;
		//Creating image views
		if (vkCreateImageView(m_device, &info, 0, &m_swapChainImageViews[i]) != VK_SUCCESS)
			throw std::runtime_error("Could not create image view");
	}
	return;
}

void Vulkan::CreateRenderPass()
{
	//Specify color attachment
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = m_swapChainFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	//Specify attachment reference
	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	//Subpass creation
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcAccessMask = 0;
	dependency.srcStageMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	

	VkRenderPassCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.attachmentCount = 1;
	info.pAttachments = &colorAttachment;
	info.subpassCount = 1;
	info.pSubpasses = &subpass;
	info.dependencyCount = 1;
	info.pDependencies = &dependency;
	
	if (vkCreateRenderPass(m_device, &info, 0, &m_renderPass) != VK_SUCCESS)
		throw std::runtime_error("could not create render pass");

	return;
}

void Vulkan::CreateGraphicsPipeline()
{
	//Create modules
	VkShaderModule vShaderModule = CreateShaderModule("shaders/vert.spv");
	VkShaderModule fShaderModule = CreateShaderModule("shaders/frag.spv");
	//Vertex shader stage info
	VkPipelineShaderStageCreateInfo vShaderStageCreationInfo{};
	vShaderStageCreationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vShaderStageCreationInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vShaderStageCreationInfo.module = vShaderModule;	
	vShaderStageCreationInfo.pName = "main";
	//Fragment shader stage info
	VkPipelineShaderStageCreateInfo fShaderStageCreationInfo{};
	fShaderStageCreationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fShaderStageCreationInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fShaderStageCreationInfo.module = fShaderModule;	
	fShaderStageCreationInfo.pName = "main";	
	VkPipelineShaderStageCreateInfo shaderStages[] = { vShaderStageCreationInfo,fShaderStageCreationInfo };
	//Vertex input creation info
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{}; 
	auto BindingDesc = Vertex::getBindingDescription();
	auto AttributeDesc = Vertex::getAttributeDescription();
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &BindingDesc;
	vertexInputInfo.vertexAttributeDescriptionCount = AttributeDesc.size();
	vertexInputInfo.pVertexAttributeDescriptions = AttributeDesc.data();
	//Input assembly creation info
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	//Specifying viewport
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = m_swapChainExtent.width;
	viewport.height = m_swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	//Specifying scissor
	VkRect2D scissor{};
	scissor.offset = { 0,0 };
	scissor.extent = m_swapChainExtent;
	//Viewport creation info
	VkPipelineViewportStateCreateInfo viewportInfo{};
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.viewportCount = 1;
	viewportInfo.pViewports = &viewport;
	viewportInfo.scissorCount = 1;
	viewportInfo.pScissors = &scissor;
	//Rasterizer creation info
	VkPipelineRasterizationStateCreateInfo rasterInfo{};
	rasterInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterInfo.depthClampEnable = VK_FALSE;
	rasterInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterInfo.lineWidth = 1.0f;
	rasterInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterInfo.depthBiasEnable = VK_FALSE;
	rasterInfo.depthBiasConstantFactor = 0.0f;
	rasterInfo.depthBiasClamp = 0.0f;
	rasterInfo.depthBiasSlopeFactor = 0.0f;
	VkPipelineMultisampleStateCreateInfo multisamplingInfo{};
	multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingInfo.sampleShadingEnable = VK_FALSE;
	multisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisamplingInfo.minSampleShading = 1.0f;
	multisamplingInfo.pSampleMask = 0;
	multisamplingInfo.alphaToCoverageEnable = VK_FALSE;
	multisamplingInfo.alphaToOneEnable = VK_FALSE;
	//Color blending info
	VkPipelineColorBlendAttachmentState colorBlendAttachmentInfo{};
	colorBlendAttachmentInfo.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachmentInfo.blendEnable = VK_FALSE;
	colorBlendAttachmentInfo.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentInfo.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentInfo.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentInfo.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentInfo.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentInfo.alphaBlendOp = VK_BLEND_OP_ADD;
	//More color blending info
	VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
	colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendInfo.logicOpEnable = VK_FALSE;
	colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendInfo.attachmentCount = 1;
	colorBlendInfo.pAttachments = &colorBlendAttachmentInfo;
	colorBlendInfo.blendConstants[0] = 0.0f;
	colorBlendInfo.blendConstants[1] = 0.0f;
	colorBlendInfo.blendConstants[2] = 0.0f;
	colorBlendInfo.blendConstants[3] = 0.0f;
	//Specifying dynamic states
	VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.dynamicStateCount = 0;
	dynamicStateInfo.pDynamicStates = 0;

	VkPipelineLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.setLayoutCount = 0;
	layoutInfo.pSetLayouts = 0;
	layoutInfo.pushConstantRangeCount = 0;
	layoutInfo.pPushConstantRanges = 0;

	if (vkCreatePipelineLayout(m_device, &layoutInfo, 0, &m_pipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("Could not create pipeline layout");
	VkGraphicsPipelineCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	info.stageCount = 2;
	info.pStages = shaderStages;
	info.pVertexInputState = &vertexInputInfo;
	info.pInputAssemblyState = &inputAssembly;
	info.pViewportState = &viewportInfo;
	info.pRasterizationState = &rasterInfo;
	info.pMultisampleState = &multisamplingInfo;
	info.pDepthStencilState = 0;
	info.pColorBlendState = &colorBlendInfo;
	info.pDynamicState = &dynamicStateInfo;
	info.layout = m_pipelineLayout;
	info.renderPass = m_renderPass;
	info.subpass = 0;
	info.basePipelineHandle = VK_NULL_HANDLE;
	info.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &info, 0, &m_graphicsPipeline) != VK_SUCCESS)
	{
		//Preventing memmory leak
		vkDestroyShaderModule(m_device, vShaderModule, 0);
		vkDestroyShaderModule(m_device, fShaderModule, 0);
		throw std::runtime_error("Could not create render pipeline");
	}
	//Destroy modules
	vkDestroyShaderModule(m_device, vShaderModule, 0);
	vkDestroyShaderModule(m_device, fShaderModule, 0);
	return;
}

VkShaderModule Vulkan::CreateShaderModule(const char* path)
{
	//open file
	std::ifstream f(path,std::ios::ate | std::ios::binary);
	if (!f.is_open()) 
		throw std::runtime_error("Could not open file");
	//Get file size
	size_t fileSize = (size_t)f.tellg();
	std::vector<char> data(fileSize);
	//go to beggining
	f.seekg(0);
	//read data
	f.read(data.data(), fileSize);
	//close file
	f.close();
	//Getting all the info
	VkShaderModuleCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.codeSize = data.size();
	info.pCode = reinterpret_cast<const uint32_t*> (data.data());
	VkShaderModule sModule;
	//Create shader module
	if (vkCreateShaderModule(m_device, &info, 0, &sModule) != VK_SUCCESS)
		throw std::runtime_error("Could not create shader module");
	return sModule;
}

void Vulkan::CreateFrameBuffers()
{
	m_swapChainFrameBuffers.resize(m_swapChainImageViews.size());
	for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
	{
		VkImageView attachments[] = { m_swapChainImageViews[i] };
		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.renderPass = m_renderPass;
		info.attachmentCount = 1;
		info.pAttachments = attachments;
		info.width = m_swapChainExtent.width;
		info.height = m_swapChainExtent.height;
		info.layers = 1;
		if (vkCreateFramebuffer(m_device, &info, 0, &m_swapChainFrameBuffers[i]) != VK_SUCCESS)
			throw std::runtime_error("Could not create frame buffer");
	}
	return;
}

void Vulkan::CreateCommandPools()
{
	QueueFamilyIndecies indecies = findQueueFamilies(m_physicalDevice);
	
	VkCommandPoolCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	info.queueFamilyIndex = indecies.graphicsFamily.value();
	info.flags = 0;
	if (vkCreateCommandPool(m_device, &info, 0, &m_commandPool) != VK_SUCCESS)
		throw std::runtime_error("Could not create command pool");
	info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	if (vkCreateCommandPool(m_device, &info, 0, &m_commandPool1) != VK_SUCCESS)
		throw std::runtime_error("Could not create command pool");
	return;
}

void Vulkan::CreateCommandBuffers()
{
	m_commandBuffers.resize(m_swapChainFrameBuffers.size());
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandBufferCount = m_commandBuffers.size();
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	
	if (vkAllocateCommandBuffers(m_device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("Could not allocate command buffers");

	for (size_t i = 0; i < m_commandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pInheritanceInfo = 0;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("Could not begin command buffer");
		//Setting all commands
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_renderPass;
		renderPassInfo.framebuffer = m_swapChainFrameBuffers[i];
		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = m_swapChainExtent;

		VkClearValue clearColor = { {{84.0f / 256,190.0f / 256,236.0f / 256,1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;
		vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
		VkBuffer vertexBuffers[] = { m_vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);
		vkCmdDraw(m_commandBuffers[i], vertices.size(), 1, 0, 0);
		vkCmdEndRenderPass(m_commandBuffers[i]);
		if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
			throw std::runtime_error("Could not record command buffer");
	}
	return;
}

void Vulkan::CreateSyncObjects()
{
	VkSemaphoreCreateInfo sInfo{};
	sInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	sInfo.flags = 0;
	VkFenceCreateInfo fInfo{};
	fInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	m_inFlightFences.resize(m_settings.maxFramesInFlight);
	m_imageAvailableSemaphores.resize(m_settings.maxFramesInFlight);
	m_renderFinishedSemaphores.resize(m_settings.maxFramesInFlight);
	m_imagesInFlight.resize(m_swapChainImages.size(),VK_NULL_HANDLE);
	for (size_t i = 0; i < m_settings.maxFramesInFlight; i++)
		if (vkCreateSemaphore(m_device, &sInfo, 0, &m_imageAvailableSemaphores[i]) != VK_SUCCESS
			||
			vkCreateSemaphore(m_device, &sInfo, 0, &m_renderFinishedSemaphores[i]) != VK_SUCCESS
			||
			vkCreateFence(m_device, &fInfo, 0, &m_inFlightFences[i]) != VK_SUCCESS)
			throw std::runtime_error("Could not create  sync objects");

	return;

}


void Vulkan::RecreateSwapChain()
{
	//Check if window is minimised
	int w = 0, h = 0;
	glfwGetFramebufferSize(m_pWnd, &w, &h);
	//wait for it to unminimize
	while (w == 0 ||h== 0)
	{
		glfwGetFramebufferSize(m_pWnd, &w, &h);
		glfwWaitEvents();
	}

	//Wait for everything to finish
	vkDeviceWaitIdle(m_device);
	CleanupSwapChain();
	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	CreateGraphicsPipeline();
	CreateFrameBuffers();
	CreateCommandBuffers();
	return;
}

void Vulkan::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& Buffer, VkDeviceMemory& memory)
{
	VkBufferCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.size = size;
	info.usage = usage;
	info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	info.flags = 0;
	if (vkCreateBuffer(m_device, &info, 0, &Buffer) != VK_SUCCESS)
		throw std::runtime_error("Could not create buffer");

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_device, Buffer, &memRequirements);
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits,properties);
	if (vkAllocateMemory(m_device, &allocInfo, 0, &memory) != VK_SUCCESS)
		throw std::runtime_error("Could not allocate vertex buffer memory");
	vkBindBufferMemory(m_device, Buffer,memory, 0);
	return;
}

void Vulkan::CopyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_commandPool1;
	allocInfo.commandBufferCount = 1;
	VkCommandBuffer cmdBuffer;
	if (vkAllocateCommandBuffers(m_device, &allocInfo, &cmdBuffer) != VK_SUCCESS)
		throw std::runtime_error("Could not allocate command buffer");
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	if (vkBeginCommandBuffer(cmdBuffer, &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("Could not begin command buffer");
	VkBufferCopy cpyReg{};
	cpyReg.srcOffset = 0;
	cpyReg.dstOffset = 0;
	cpyReg.size = size;
	vkCmdCopyBuffer(cmdBuffer, src, dst, 1, &cpyReg);
	if (vkEndCommandBuffer(cmdBuffer) != VK_SUCCESS)
		throw std::runtime_error("Could not end command buffer");

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmdBuffer;	
	vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, 0);
	vkQueueWaitIdle(m_graphicsQueue);
	vkFreeCommandBuffers(m_device, m_commandPool1, 1, &cmdBuffer);

		
	return;
}

void Vulkan::CreateVertexBuffer()
{
	VkDeviceSize size = sizeof(vertices[0]) * vertices.size();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
	//Fill the buffer
	void* data;
	//Map memory
	vkMapMemory(m_device, stagingBufferMemory, 0, size, 0, &data);
	//Copy verticies to the memory
	memcpy(data, vertices.data(), size);
	//Unmap memory
	vkUnmapMemory(m_device, stagingBufferMemory);
	
	CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);
	CopyBuffer(stagingBuffer, m_vertexBuffer, size);
	vkDestroyBuffer(m_device, stagingBuffer, 0);
	vkFreeMemory(m_device, stagingBufferMemory, 0);
	return;
}

uint32_t Vulkan::FindMemoryType(uint32_t typeFilter,VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProp;
	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProp);

	for (uint32_t i = 0; i < memProp.memoryTypeCount; i++)	
		if ((typeFilter & (1 << i)) && (memProp.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	throw std::runtime_error("Could not find suitable memory type");
	return 0;
}

void Vulkan::DrawFrame()
{
	vkWaitForFences(m_device, 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	//Get next image
	VkResult res = vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);
	if(res == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return;
	}
	else if(res != VK_SUCCESS&& res != VK_SUBOPTIMAL_KHR)	
		throw std::runtime_error("Could not aquire swap chain image");	
	if (m_imagesInFlight[imageIndex] != VK_NULL_HANDLE)
		vkWaitForFences(m_device, 1, &m_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	m_imagesInFlight[imageIndex] = m_inFlightFences[m_currentFrame];

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphores[m_currentFrame] };
	VkPipelineStageFlags waitSages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores =  waitSemaphores;
	submitInfo.pWaitDstStageMask = waitSages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];
	VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;
	
	vkResetFences(m_device, 1, &m_inFlightFences[m_currentFrame]);

	if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo,m_inFlightFences[m_currentFrame]) != VK_SUCCESS)
		throw std::runtime_error("Could not submit draw command buffer");

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { m_swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = 0;
	//Present 
	res = vkQueuePresentKHR(m_presentQueue, &presentInfo);
	if (res == VK_ERROR_OUT_OF_DATE_KHR || VK_SUBOPTIMAL_KHR || m_resized)
	{
		RecreateSwapChain();
		m_resized = false;
	}
	else if (res != VK_SUCCESS)
		throw std::runtime_error("Could not present image");
	

	m_currentFrame = (m_currentFrame + 1) % m_settings.maxFramesInFlight;
	return;
}

std::vector<const char*> Vulkan::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	//If debuging is enabled add this extension
	if (enableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);


	return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Vulkan::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverety, VkDebugUtilsMessageTypeFlagBitsEXT msgType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	//Display message
	switch (msgSeverety)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		return VK_FALSE;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		std::cerr << "Validation layer: " << "[info] ";
		
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		std::cerr << "Validation layer: " << "[warning] ";
		
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		std::cerr << "Validation layer: " << "[eror] ";
		
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
		return VK_FALSE;
		break;
	default:
		return VK_FALSE;
		break;
	}
	std::cerr << pCallbackData->pMessage << '\n';
	
	return VK_FALSE;
}

VkVertexInputBindingDescription Vertex::getBindingDescription()
{
	VkVertexInputBindingDescription desc{};
	desc.binding = 0;
	desc.stride = sizeof(Vertex);
	desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return desc;
}

std::vector<VkVertexInputAttributeDescription> Vertex::getAttributeDescription()
{
	std::vector< VkVertexInputAttributeDescription> desc(2);	
	desc[0].binding = 0;
	desc[0].location = 0;
	desc[0].format = VK_FORMAT_R32G32_SFLOAT;
	desc[0].offset = offsetof(Vertex, Position);
	desc[1].binding = 0;
	desc[1].location = 1;
	desc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	desc[1].offset = offsetof(Vertex, Color);
	return desc;
}
