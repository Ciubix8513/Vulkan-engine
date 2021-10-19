#include "System.h"

System::System()
{
	m_wnd = 0;
	m_vulkan = 0;
	m_FPSupdateInterval = 1;
	m_time = 0;
	m_input = 0;
}

void System::Run()
{
	
	m_time = new Time();
	m_time->m_begin = std::chrono::system_clock::now();
	int a = 1;
	InitWindow(800, 600);
	m_input = new Input();
	
	m_vulkan = new Vulkan();
	//lol
	Settings s{};
	s.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
	s.swapChainImgCount = 3;
	s.maxFramesInFlight = 2;
	s.TimePtr = m_time;

	m_vulkan->Init(m_wnd,s);
	std::cout << "Finished initialisation, intialisation took: " << ((std::chrono::duration<double>)(std::chrono::system_clock::now() - m_time->m_begin)).count()  <<'\n';
	//start main loop
	MainLoop();

	CleanUp();
	return;
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
	glfwSetKeyCallback(m_wnd, m_input->KeyCallback);
	return;

}

void System::UpdateFPS()
{
	//TODO Add calculation of average fps beetween title updates
	while (!glfwWindowShouldClose(m_wnd))
	{
		float fps = 1 / m_time->GetDeltaT();
		std::ostringstream ss;
		ss << fps;
		glfwSetWindowTitle(m_wnd, ((std::string)"Vulkan Engine  FPS: " + ss.str()).c_str());
		std::this_thread::sleep_for(std::chrono::seconds(m_FPSupdateInterval));
		
	}
	return;
}

void System::MainLoop()
{
	std::thread TitleUpdateThread([this] { this->UpdateFPS(); });

	//Run until recive window close flag
	while (!glfwWindowShouldClose(m_wnd))	
	{
		
		//process window events
		glfwPollEvents();
		//Update input
		m_input->UpdateKeys();
		//Update time and dTime
		m_time->Update();
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
	
	delete m_time;
	m_time = 0;
	return;

}





void System::frameBufferResizeCallBack(GLFWwindow* wnd, int w, int h)
{
	auto app = (System*)glfwGetWindowUserPointer(wnd);
	app->m_vulkan->m_resized = true;
	return;
}


