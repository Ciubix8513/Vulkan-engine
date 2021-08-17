#include <iostream>
#include "System.h"

int main ()
{		
	std::cout << "Application start\n";
	System* system = new System;
	system->Run();
	delete system;
	std::cout << "Application finished";
	std::cin.get();
	return 0;
}