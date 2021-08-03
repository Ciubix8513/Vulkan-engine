#pragma once
#ifndef _MATRIX4X4_H_
#define _MATRIX4X4_H_
#include "../Vectors/Quarernion.h"

using namespace EngineMath;
namespace EngineMath 
{
	class Matrix4x4
	{

	private:
		Matrix4x4 Identity();

	public:
#pragma region Creation + constants
		Matrix4x4( float m00, float m01, float m02, float m03,
				   float m10, float m11, float m12, float m13,
				   float m20, float m21, float m22, float m23,
			       float m30, float m31, float m32, float m33);
		Matrix4x4(Vector4 row0, Vector4 row1, Vector4 row2, Vector4 row3);
		Matrix4x4();
#pragma endregion
#pragma region Functions
		Matrix4x4 Transposed();
		void Transpose();
		
		float getDeterminant();
		float trace();
		
		void SetRow(int row, Vector4 value);
		Vector4 TransformVector(Vector4 v);
		Matrix4x4 Inversed();
			
	
#pragma endregion


#pragma region Operators
		Matrix4x4 operator+(Matrix4x4 m);
		Matrix4x4 operator-(Matrix4x4 m);
		Matrix4x4 operator* (float c);
		Matrix4x4 operator/(float c);
		Matrix4x4 operator* (Matrix4x4 m);
		void operator+=(Matrix4x4 m);
		void operator-=(Matrix4x4 m);
		void operator*= (float c);
		void operator/=(float c);
		void operator= (Matrix4x4 m);
		Vector4 operator[] (int index);
		Vector4 operator* (Vector4 v);
		bool operator== (Matrix4x4);
		//operator DirectX::XMFLOAT4X4() ;
	
#pragma endregion



		float 
			_m00, _m01, _m02, _m03,
			_m10, _m11, _m12, _m13,
			_m20, _m21, _m22, _m23,
			_m30, _m31, _m32, _m33;



	private:
		Vector4 GetRow(int index);

	};

}
#endif 

