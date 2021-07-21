#include <iostream>
#include "System.h"

int main ()
{	
	std::cout << "Engine start\n";
	System* system = new System;
	system->Run();
	delete system;
	return 0;
}