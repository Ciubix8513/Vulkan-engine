#pragma once
#ifndef _MODEL_LOADER_H_
#define _MODEL_LOADER_H_
#include <fstream>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "Math/EngineMath.h"
class ModelLoader
{

public:
	struct Vertex
	{
		EngineMath::Vector3 Position;
		EngineMath::Vector2 UV;
		EngineMath::Vector3 Normal;
	};
	struct Mesh
	{
		Vertex* Vertices;
		unsigned int* Indecies;
		unsigned int numVertecies;
		unsigned int numIndecies;
	};
	static Mesh LoadOBJ(std::string path);

};
#endif // !_MODEL_LOADER_H_

