#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <iostream>

int main ()
{	
	std::cout << "Engine start\n ";
	
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* wnd = 0;
	wnd = glfwCreateWindow(800, 600, "Vulkan engine,", 0, 0);

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(0, &extensionCount, 0);

	std::cout << extensionCount << '\n';

	while (!glfwWindowShouldClose(wnd))
	{
		glfwPollEvents();
	}

	glfwDestroyWindow(wnd);
	glfwTerminate();

	return 0;
}