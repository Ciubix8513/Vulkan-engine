#include <iostream>
#include "System.h"
#include "ModelLoader.h"


	int main()
	{
		


		auto a = a::ModelLoader::ConvertOBJ("data/girl.obj");
		a::ModelLoader::SaveModel(a, "data/girl.lmdl");
		auto b = a::ModelLoader::LoadModel("data/girl.lmdl");
		for (size_t i = 0; i < a->numIndecies; i++)
			if (a->Indecies[i] != b->Indecies[i])
				break;
		for (size_t i = 0; i < a->numVerticies; i++)
			if (a->Vertices[i].Position != b->Vertices[i].Position)
				break;
		delete[] a->Indecies, b->Indecies, a->Vertices, b->Vertices;
		delete a, b;
		std::cout << "Application start\n";
		System* system = new System;
		system->Run();
		delete system;
		std::cout << "Application finished";
		std::cin.get();
		return 0;
	}
