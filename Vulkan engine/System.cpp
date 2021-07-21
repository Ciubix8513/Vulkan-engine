#include "System.h"

System::System()
{
	m_wnd = 0;
}

void System::Run()
{

	InitWindow(800, 600);
	InitVulkan();
	//start main loop
	MainLoop();

	CleanUp();
}

void System::InitWindow(size_t width, size_t height)
{
	//Initialize glfw
	glfwInit();
	//Set hints
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //Make a non OpenGL window
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //Make window non resizable 
	//Create window
	m_wnd = glfwCreateWindow(width, height, "Vulkan engine", 0, 0);
	return;

}

void System::InitVulkan()
{
	//Create instance
	CreateInstance();
}

void System::MainLoop()
{
	//Run until recive window close flag
	while (!glfwWindowShouldClose(m_wnd))	
		//process window events
		glfwPollEvents();
	return;
	
}

void System::CleanUp()
{

	//Destroy instance
	vkDestroyInstance(m_instance, 0);
	//Destroy window
	glfwDestroyWindow(m_wnd);
	//Terminate glfw
	glfwTerminate();
	

}

void System::CreateInstance()
{
	//Chech if validation layers are avalable
	if (enableValidationLayers && !CheckValidationLevelSupport()) 
	{	
		throw std::runtime_error("Requested validation layers are not available");
	}

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

	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
		createInfo.enabledLayerCount = 0;

	//Create instance
	if (vkCreateInstance(&createInfo, 0, &m_instance) != VK_SUCCESS)	
		throw std::runtime_error("Failed to create instance");
	return;
}

bool System::CheckValidationLevelSupport()
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
			if (strcmp(validationLayers[i] , availableLayers[j].layerName)) 
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

std::vector<const char*> System::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	
	//If debuging is enabled add this extension
	if(enableValidationLayers)	
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	
		 
	return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL System::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT msgeSeverety, VkDebugUtilsMessageTypeFlagBitsEXT msgType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
//U LEFT HERE! lol	
	std::cerr << "Validation layer: " << pCallbackData->pMessage << '\n';
	return VK_FALSE;
}
