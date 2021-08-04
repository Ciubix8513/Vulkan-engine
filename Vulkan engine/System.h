#define GLFW_INCLUDE_VULKAN
#include"Vulkan.h"
#include <sstream>
#include<chrono>
#include <GLFW/glfw3.h>

#ifndef _SYSTEM_H_
#define _SYSTEM_H_




class System
{
public:
	System();
	void Run();
private:
	void InitWindow(size_t width = 800, size_t height = 600);
	void MainLoop();
	void CleanUp();
	static void frameBufferResizeCallBack(GLFWwindow* wnd, int w, int h);
	
	std::chrono::time_point<std::chrono::system_clock> lastTime;
	GLFWwindow* m_wnd;
	Vulkan* m_vulkan;
};


#endif 


