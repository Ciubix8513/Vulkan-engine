#ifndef _INPUT_H_

#define _INPUT_H_
#include <vector>
#include <GLFW/glfw3.h>
#include "System.h"
class Input
{
public:
	Input();
private:
	void UpdateKeys();
	static void KeyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods);

	static const short NumKeys = 120;
	bool PressedKeys[120];
	bool HeldKeys[120];
	bool ReleasedKeys[120];
	std::vector<int> PressedUpdateKeys;
	std::vector<int> ReleasedUpdateKeys;
	friend class System;
};
#endif 

