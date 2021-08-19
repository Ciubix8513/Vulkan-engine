#define GLFW_INCLUDE_VULKAN
#include"Vulkan.h"
#include <sstream>
#include <thread>
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
	void UpdateFPS();
	void InitWindow(size_t width = 800, size_t height = 600);
	void MainLoop();
	void CleanUp();
	static void KeyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods);
	static void frameBufferResizeCallBack(GLFWwindow* wnd, int w, int h);
	
	
	int m_FPSupdateInterval;
	Time* m_time;
	GLFWwindow* m_wnd;
	Vulkan* m_vulkan;
};


#endif 


