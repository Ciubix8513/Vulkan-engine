#include "System.h"

System::System()
{
	m_wnd = 0;
	m_vulkan = 0;
	m_FPSupdateInterval = 1;
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
	m_lastTime = std::chrono::system_clock::now();
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

void System::UpdateFPS()
{
	while (!glfwWindowShouldClose(m_wnd))
	{
		float fps = 1 / m_deltaTime;
		std::ostringstream ss;
		ss << fps;
		glfwSetWindowTitle(m_wnd, ((std::string)"Vulkan engine   FPS: " + ss.str()).c_str());
		std::this_thread::sleep_for(std::chrono::seconds(m_FPSupdateInterval));

	}
}

void System::MainLoop()
{
	std::thread TitleUpdateThread([this] { this->UpdateFPS(); });

	//Run until recive window close flag
	while (!glfwWindowShouldClose(m_wnd))	
	{
		//process window events
		glfwPollEvents();
		//Update time and dTime
		UpdateTime();	
		//Draw frame
		m_vulkan->DrawFrame();
	}
	TitleUpdateThread.join();
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

void System::UpdateTime()
{
	auto time = std::chrono::system_clock::now();
	std::chrono::duration<double> SecSinceLastFrame = time - m_lastTime;
	m_deltaTime = SecSinceLastFrame.count();
	m_lastTime = time;
	
}



void System::frameBufferResizeCallBack(GLFWwindow* wnd, int w, int h)
{
	auto app = (System*)glfwGetWindowUserPointer(wnd);
	app->m_vulkan->m_resized = true;
}
