#include "System.h"

System::System()
{
	m_wnd = 0;
	m_vulkan = 0;
}

void System::Run()
{
	int a = 1;
	InitWindow(800, 600);
	m_vulkan = new Vulkan();
	//lol
	Settings s{};
	s.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
	s.swapChainImgCount = 3;
	s.maxFramesInFlight = 2;

	m_vulkan->Init(m_wnd,s);
	std::cout << "Finished Vulkan initialisation\n";
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
	//Create window
	m_wnd = glfwCreateWindow(width, height, "Vulkan engine", 0, 0);
	glfwSetWindowUserPointer(m_wnd, this);
	glfwSetFramebufferSizeCallback(m_wnd, frameBufferResizeCallBack);

	return;

}

void System::MainLoop()
{
	//Run until recive window close flag
	while (!glfwWindowShouldClose(m_wnd))	
	{
		//process window events
		glfwPollEvents();
		//Draw frame
		m_vulkan->DrawFrame();
	}
	return;
	
}

void System::CleanUp()
{

	m_vulkan->Shutdown();
	delete m_vulkan;
	m_vulkan = 0;

	//Destroy window
	glfwDestroyWindow(m_wnd);
	//Terminate glfw
	glfwTerminate();

}

void System::frameBufferResizeCallBack(GLFWwindow* wnd, int w, int h)
{
	auto app = (System*)glfwGetWindowUserPointer(wnd);
	app->m_vulkan->m_resized = true;
}
