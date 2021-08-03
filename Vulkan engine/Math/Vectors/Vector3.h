#pragma once
#ifndef _VECTOR3_H_
#define _VECTOR3_H_
#include "Vector2.h"
using namespace EngineMath;

namespace EngineMath 
{

	struct Vector3
	{
	public:
#pragma region VectorCreation + Consts
		Vector3(float NewX, float NewY, float NewZ);
		Vector3(Vector2 NewXY, float NewZ);
		Vector3(float NewX, Vector2 NewYZ);
		Vector3();

		static Vector3 Up();
		static Vector3 Down();
		static Vector3 Forward();
		static Vector3 Backward();
		static Vector3 Right();
		static Vector3 Left();
		static Vector3 Zero();
		
#pragma endregion

#pragma region Functions
		float Length();
		static float Length(Vector3& v);
		Vector3 Normalized();
		void Normalize();
		static float DotProduct(Vector3 a, Vector3 b);
		static Vector3 CrossProdut(Vector3 a, Vector3 b);
#pragma endregion

#pragma region operators
		Vector3 operator+(Vector3 v);
		Vector3 operator-(Vector3 v);
		Vector3 operator-(float c);
		Vector3 operator*(float c);
		float operator*(Vector3 c);

		Vector3 operator/(float c);
		void operator+=(Vector3 v);
		void operator-=(Vector3 v);
		void operator*=(float c);
		void operator/=(float c);
		 operator DirectX::XMFLOAT3() ;

		bool operator!=(Vector3 v);
#pragma endregion


		float x, y, z;
	};
};
#endif 
