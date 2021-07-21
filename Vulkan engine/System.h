#define GLFW_INCLUDE_VULKAN
#include <stdexcept>
#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

class System
{
public:
	System();
	void Run();
private:
	void InitWindow(size_t width = 800, size_t height = 600);
	void InitVulkan();
	void MainLoop();
	void CleanUp();
	void CreateInstance();
	
	bool CheckValidationLevelSupport();

	std::vector<const char*> getRequiredExtensions();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT msgeSeverety, VkDebugUtilsMessageTypeFlagBitsEXT msgType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	GLFWwindow* m_wnd;
	VkInstance m_instance;

	const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
};


#endif 


