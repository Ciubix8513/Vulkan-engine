#include "Input.h"

Input::Input()
{
}

void Input::UpdateKeys()
{
	for (size_t i = 0; i < PressedUpdateKeys.size(); i++)
	{
		HeldKeys[PressedUpdateKeys[i]] = true;
		PressedKeys[PressedUpdateKeys[i]] = false;
	}
	for (size_t i = 0; i < ReleasedUpdateKeys.size(); i++)
	{
		ReleasedKeys[ReleasedUpdateKeys[i]] = false;
	}
	ReleasedUpdateKeys.clear();
	PressedUpdateKeys.clear();
	return;
}

void Input::KeyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods)
{
	auto Input =( (System*)glfwGetWindowUserPointer(wnd))->m_input;
	if (action == GLFW_PRESS)
	{
		Input->HeldKeys[key % NumKeys] = false;
		Input->ReleasedUpdateKeys.push_back(key % NumKeys);
		Input->ReleasedKeys[key % NumKeys] = false;
	}
	else if (action == GLFW_PRESS)
	{
		Input->PressedKeys[key % NumKeys] = true;
		Input->PressedUpdateKeys.push_back(key % NumKeys);
	}
	return;
}
