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
	void UpdateTime();
	static void frameBufferResizeCallBack(GLFWwindow* wnd, int w, int h);
	
	std::chrono::time_point<std::chrono::system_clock> m_lastTime;
	double m_deltaTime;
	int m_FPSupdateInterval;
	GLFWwindow* m_wnd;
	Vulkan* m_vulkan;
};


#endif 


