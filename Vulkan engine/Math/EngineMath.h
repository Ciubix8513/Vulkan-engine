#pragma once
#ifndef _ENGINE_MATH_H_
#define _ENGINE_MATH_H_
#include "Vectors/Plane.h"
using namespace EngineMath;

namespace EngineMath
{

	Matrix4x4 Multiply(Matrix4x4 a, Matrix4x4 b);
	Vector4 TransformVector(Vector4 v, Matrix4x4 m);
	Matrix4x4 PerspectiveProjectionMatrix(float FOV, float  screenAspect, float  screenNear, float  screenDepth);
	Matrix4x4 OrthographicProjectionMatrix(float screenWidth, float screenHeight, float screenNear, float screenDepth);
	Matrix4x4 Transpose(Matrix4x4 m);
	Matrix4x4 Identity();

	Matrix4x4 LookAtMatrix(Vector3 CameraPosition, Vector3 CameraForward, Vector3 cameraUp);
	Matrix4x4 RotationPitchYawRoll(float pitch, float yaw, float roll);
	Matrix4x4 RotationPitchYawRoll(Vector3 rpy);
	Matrix4x4 TransformationMatrix(Vector3 posistion, Vector3 Rotation, Vector3 Scale);
	Matrix4x4 TransformationMatrix(Vector3 posistion, Vector3 Rotation, float Scale);

	Matrix4x4 TranslationScaleMatrix(Vector3 translation, Vector3 scale);
	Matrix4x4 TranslationScaleMatrix(Vector3 translation, float scale);

	Matrix4x4 TranslationMatrix(Vector3 translation);
	Matrix4x4 ScaleMatrix(Vector3 scale);
	Matrix4x4 ScaleMatrix(float scale);
#pragma region plain funcs
	float PlaneDotCoord(Plane plane, Vector3 point);
	Vector3 Normalized(Vector3 v);
	
#pragma endregion

}
#endif
