#pragma once
#ifndef _MODEL_LOADER_H_
#define _MODEL_LOADER_H_
#include <fstream>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstdint>
#include <optional>
#include <unordered_map>
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
	//Obj file MUST contain ONLY 1(ONE) object
	static Mesh LoadOBJ(std::string path);
	static Vector3int* Triangulate(Vector3int* vertices, uint32_t size);
private:
	//static unsigned int Hash(Vector3int data,int tableSize);
	template<typename T>
	static bool FindValue(T* arr, uint32_t arrSize, T value, uint32_t& index)
	{
		for (size_t i = 0; i < arrSize; i++)
			if (arr[i] == value)
			{
				index = i;
				return true;
			}
		return false;
	}
};


namespace std
{

	template <> struct hash<Vector3int>
	{
		size_t operator()(Vector3int const& vector) const 
		{
			unsigned int p1, p2, p3;

			p1 = 73856093;
			p2 = 19349663;
			p3 = 83492791;
			unsigned int h = ((vector.y * p1) ^ (vector.z * p2) ^ (vector.x * p3)) ;
			return h;
		}
	};
}
#endif // !_MODEL_LOADER_H_

