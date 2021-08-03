#include "Quarernion.h"

Quaternion::Quaternion()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}
Quaternion::~Quaternion()
{
}
Quaternion::Quaternion(float X, float Y, float Z, float W)
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}
Quaternion::Quaternion(Vector4 xyzw)
{
	x = xyzw.x;
	y = xyzw.y;
	z = xyzw.z;
	w = xyzw.w;
}
Quaternion::Quaternion(Vector3 xyz, float W)
{

	x = xyz.x;
	y = xyz.y;
	z = xyz.z;
	w = W;
}
Quaternion::Quaternion(Vector2 xy, Vector2 zw)
{
	x = xy.x;
	y = xy.y;
	z = zw.x;
	w = zw.y;
}

Quaternion::Quaternion(Vector3 rotation)
{
	Vector3 rot = rotation; //* Deg2Rad;
	float cy = (float)cos(rot.y * 0.5);
	float sy = (float)sin(rot.y * 0.5);
	float cp = (float)cos(rot.x * 0.5);
	float sp = (float)sin(rot.x * 0.5);
	float cr = (float)cos(rot.z * 0.5);
	float sr = (float)sin(rot.z * 0.5);

	
	w = cr * cp * cy + sr * sp * sy;
	x = sr * cp * cy - cr * sp * sy;
	y = cr * sp * cy + sr * cp * sy;
	z = cr * cp * sy - sr * sp * cy;

	
}

Quaternion::Quaternion(float x, float y, float z)
{
	Vector3 rot = Vector3(x,y,z) * Deg2Rad;
	double cy = cos(rot.y * 0.5);
	double sy = sin(rot.y * 0.5);
	double cp = cos(rot.x * 0.5);
	double sp = sin(rot.x * 0.5);
	double cr = cos(rot.z * 0.5);
	double sr = sin(rot.z * 0.5);


	w = cr * cp * cy + sr * sp * sy;
	x = sr * cp * cy - cr * sp * sy;
	y = cr * sp * cy + sr * cp * sy;
	z = cr * cp * sy - sr * sp * cy;
}

Vector3 Quaternion::Euler()
{
	Vector3 angles;
	double sinr_cosp = 2 * (w * x + y *z);
	double cosr_cosp = 1 - 2 * (x * x + y * y);
	angles.z = std::atan2(sinr_cosp, cosr_cosp);

	// x (y-axis rotation)
	double sinp = 2 * (w * y - z * x);
	if (std::abs(sinp) >= 1)
		angles.x = std::copysign(PI / 2, sinp); // use 90 degrees if out of range
	else
		angles.x = std::asin(sinp);

	// y (z-axis rotation)
	double siny_cosp = 2 * (w * z + x * y);
	double cosy_cosp = 1 - 2 * (y * y + z * z);
	angles.y = std::atan2(siny_cosp, cosy_cosp);

	return angles;

}

bool Quaternion::operator==(Quaternion other)
{
	return x == other.x && y == other.y && z == other.z && w == other.w;
}
Quaternion Quaternion::operator*(Quaternion other)
{
	
	return Quaternion(
		other.x * x - other.y * y - other.z * z - other.w * w,
		other.x * y + other.y * x - other.z * w + other.w * z,
		other.x * z + other.y * w + other.z * x - other.w * y,
		other.x * w - other.y * z + other.z * y + other.w * x);
}
