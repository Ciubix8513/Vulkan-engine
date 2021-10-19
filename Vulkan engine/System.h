#define GLFW_INCLUDE_VULKAN
#include"Vulkan.h"
#include <sstream>
#include <thread>
#include<chrono>
#include <GLFW/glfw3.h>
#include "Input.h"
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

//Forward declaring Input to enable cross referencing
class Input;

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
	static void frameBufferResizeCallBack(GLFWwindow* wnd, int w, int h);
	
	int m_FPSupdateInterval;
	Time* m_time;
	GLFWwindow* m_wnd;
	Vulkan* m_vulkan;
	Input* m_input;
	friend class Input;
};


#endif 


