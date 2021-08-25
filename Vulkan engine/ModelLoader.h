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
	enum Attribute
	{
		POSITION = 1,
		UV = 2,
		NORMAL = 4,
		ALL = 7
	};
	typedef char Attributes;

	struct Vertex
	{
		EngineMath::Vector3 Position;
		EngineMath::Vector2 UV;
		EngineMath::Vector3 Normal;
		Vertex(Vector3 pos,Vector2 uv,Vector3 normal);
		Vertex();
	};
	struct Mesh
	{
		Vertex* Vertices;
		unsigned int* Indecies;
		unsigned int numVertecies;
		unsigned int numIndecies;
		Attributes atr;
	};
	static Mesh LoadOBJ(std::string path);
private:
	static int Hash(Vector3int data,int tableSize);

};
#endif // !_MODEL_LOADER_H_

