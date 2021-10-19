#include <iostream>
#include "System.h"
#include "ModelLoader.h"


	int main()
	{
	/*	auto  m = a::ModelLoader::ConvertOBJ("data/girl.obj");
		a::ModelLoader::SaveModel(m, "data/girl.lmdl");
		delete m.Indecies;
		delete m.Vertices;*/
std::cout << "Application start\n";
		System* system = new System;
		system->Run();
		delete system;
		std::cout << "Application finished";
		std::cin.get();
		return 0;
	}
