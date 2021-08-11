#ifndef _VULKAN_H_
#define _VULKAN_H_
#include <fstream>
#include <stdexcept>
#include <vector>
#include <optional>
#include <iostream>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <set>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include "Math/EngineMath.h"
#include "Time.h"


//Load function
static VkResult CreateDebugUtilsMessengerExt(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreatInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
		return func(instance, pCreatInfo, pAllocator, pDebugMessenger);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}
//Load function
static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
		func(instance, debugMessenger, pAllocator);
	return;
}

struct QueueFamilyIndecies
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> modes;
};

struct Settings
{
	Time* TimePtr;
	VkPresentModeKHR presentMode;
	uint32_t swapChainImgCount;
	uint32_t maxFramesInFlight;
};

struct Vertex
{
	EngineMath::Vector3 Position;
	EngineMath::Vector3 Color;
	static VkVertexInputBindingDescription getBindingDescription();
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
};

struct Matricies
{
	EngineMath::Matrix4x4 world;
	EngineMath::Matrix4x4 view;
	EngineMath::Matrix4x4 proj;
};

class Vulkan
{

public:
	Vulkan();
	void Init(GLFWwindow* wnd,Settings settings);
	void Shutdown();
	void DrawFrame();
	

private:
	void CreateInstance();
	bool CheckValidationLevelSupport();
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	void SetupDebugMessenger();
	void CreateSurface();
	void PickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndecies findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainDetails(VkPhysicalDevice device);
	void CreateLogicalDevice();
	void CreateSwapChain();
	VkSurfaceFormatKHR ChooseFormat(const std::vector<VkSurfaceFormatKHR>& formats);
	VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& modes);
	VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateImageViews();
	void CreateRenderPass();
	void CreateGraphicsPipeline();
	VkShaderModule CreateShaderModule(const char* path);
	void CreateFrameBuffers();
	void CreateCommandPools();
	void CreateCommandBuffers();
	void CreateSyncObjects();
	void CleanupSwapChain();
	void RecreateSwapChain();
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& Buffer, VkDeviceMemory& memory);
	void CopyBuffer(VkBuffer src,VkBuffer dst,VkDeviceSize size);
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void CreateDescriptorSetLayout();
	void CreateUniformBuffers();
	void UpdateUniformBuffer(uint32_t imgIndex);
	void CreateDescriptorPool();
	void CreateDescriptorSet();

	std::vector<const char*> getRequiredExtensions();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT msgeSeverety,
		VkDebugUtilsMessageTypeFlagBitsEXT msgType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

#pragma region Members

	Settings m_settings;
	VkInstance m_instance;
	VkDebugUtilsMessengerEXT m_messenger;
	VkPhysicalDevice m_physicalDevice;
	VkDevice m_device;
	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
	VkSurfaceKHR m_surface;
	VkSwapchainKHR m_swapChain;
	std::vector<VkImage> m_swapChainImages;
	VkFormat m_swapChainFormat;
	VkExtent2D m_swapChainExtent;
	std::vector<VkImageView> m_swapChainImageViews;
	VkRenderPass m_renderPass;
	VkPipelineLayout m_pipelineLayout;
	VkDescriptorSetLayout m_descSetLayout;
	VkPipeline m_graphicsPipeline;
	std::vector<VkFramebuffer> m_swapChainFrameBuffers;
	VkCommandPool m_commandPool;
	VkCommandPool m_commandPool1;
	std::vector<VkCommandBuffer> m_commandBuffers;
	std::vector < VkSemaphore> m_imageAvailableSemaphores;
	std::vector < VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;
	std::vector<VkFence> m_imagesInFlight;
	uint32_t m_currentFrame;
	GLFWwindow* m_pWnd;
	bool m_resized;
	VkBuffer m_vertexBuffer;
	VkBuffer m_indexBuffer;
	VkDescriptorPool m_descPool;
	std::vector<VkDescriptorSet> m_descSets;

	VkDeviceMemory m_vertexBufferMemory;
	VkDeviceMemory m_indexBufferMemory;

	std::vector<VkBuffer> m_uBuffers;
	std::vector<VkDeviceMemory> m_uBuffersMem;
	Quaternion rot;
	Vector3 rot3;

	//List of required device extensions
	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	//List of used validation layers
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
public:
	const std::vector<Vertex> vertices = {
	{{-0.5f, 0,-0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f,0,-0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f,0, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f,0, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};
	const std::vector<uint16_t> indecies = { 0,1,2,2,3,0 };
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
#pragma endregion
	friend class System;
};

#endif
