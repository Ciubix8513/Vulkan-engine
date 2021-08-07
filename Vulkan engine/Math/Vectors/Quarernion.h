#pragma once
#ifndef  _QUATERNION_H_
#define  _QUATERNION_H_
#include "Vector4.h"
#include <cmath>

class Quaternion
{
public:
	Quaternion();
	~Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(Vector4 xyzw);
	Quaternion(Vector3 xyz, float w);
	Quaternion(Vector2 xy, Vector2 zw);
	Quaternion(Vector3 rotation);
	Quaternion(float x, float y, float z);
	static Quaternion Slerp(Quaternion a, Quaternion b, float t);
	Vector3 Euler();
	Quaternion Inverse();
	static Quaternion Invert(Quaternion q);
	Quaternion Conjugate();
	float Magnitude();

	bool operator==(Quaternion other);
	Quaternion operator/ (float f);
	Quaternion operator*(Quaternion other);
	Quaternion operator*(float f);
	Quaternion operator+(Quaternion b);
public:
	float x, y, z, w;

};

#endif 
