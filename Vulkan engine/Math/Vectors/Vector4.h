#pragma once
#ifndef _VECTOR4_H_
#define _VECTOR4_H_
#include "Vector3.h"
using namespace EngineMath;

namespace EngineMath
{

	struct Vector4
	{
	public:
#pragma region VectorCreation + Consts
		Vector4(float NewX, float NewY, float NewZ, float NewW);
		Vector4(Vector2 NewXY, float NewZ, float NewW); 
		Vector4(Vector2 NewXY, Vector2 NewZW);
		Vector4(float NewX, Vector2 NewYZ, float NewW);
		Vector4(float NewX, float NewY, Vector2 NewZW);
		Vector4(float NewX, Vector3 NewYZW);
		Vector4(Vector3 NewXYZ, float NewW);
		Vector4();



		static Vector4 Up();
		static Vector4 Down();
		static Vector4 Forward();
		static Vector4 Backward();
		static Vector4 Right();
		static Vector4 Left();
		static Vector4 Zero();
#pragma endregion

#pragma region Functions
		float Length();
		static float Length(Vector4& v);
		Vector4 Normalized();
		static void Normalize(Vector4& v);
		void Normalize();
		static float DotProduct(Vector4 a, Vector4 b);
		Vector3 XYZ();
		Vector2 XY();
#pragma endregion

#pragma region operators

		Vector4 operator+(Vector4 v);
		Vector4 operator-(Vector4 v);
		Vector4 operator*(float c);
		Vector4 operator/(float c);
		float operator*(Vector4 v);
		float operator[](int index);
		void operator+=(Vector3 v);
		void operator+=(Vector4 v);
		void operator-=(Vector4 v);
		void operator*=(float c);
		void operator/=(float c);
 operator DirectX::XMFLOAT4();

#pragma endregion


		float x, y, z,w;
	};
};
#endif 
