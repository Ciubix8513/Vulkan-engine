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

//Stole some code from euclidianspace.com but shhhh.... don't tell anyone lol  :)
Quaternion Quaternion::Slerp(Quaternion qa, Quaternion qb, float t)
{
	//Clamp t
	if (t > 1) t = 1;
	if (t < 0) t = 0;

	// quaternion to return
	Quaternion qm;
	// Calculate angle between them.
	double cosHalfTheta = qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if (abs(cosHalfTheta) >= 1.0) {
		qm.w = qa.w; qm.x = qa.x; qm.y = qa.y; qm.z = qa.z;
		return qm;
	}
	// Calculate temporary values.
	double halfTheta = acos(cosHalfTheta);
	double sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	if (fabs(sinHalfTheta) < 0.001) { // fabs is floating point absolute
		qm.w = (qa.w * 0.5 + qb.w * 0.5);
		qm.x = (qa.x * 0.5 + qb.x * 0.5);
		qm.y = (qa.y * 0.5 + qb.y * 0.5);
		qm.z = (qa.z * 0.5 + qb.z * 0.5);
		return qm;
	}
	double ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
	double ratioB = sin(t * halfTheta) / sinHalfTheta;
	//calculate Quaternion.
	qm.w = (qa.w * ratioA + qb.w * ratioB);
	qm.x = (qa.x * ratioA + qb.x * ratioB);
	qm.y = (qa.y * ratioA + qb.y * ratioB);
	qm.z = (qa.z * ratioA + qb.z * ratioB);
	return qm;
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

Quaternion Quaternion::Inverse()
{	

	return Conjugate() / pow(Magnitude(), 2);
}

Quaternion Quaternion::Invert(Quaternion q)
{
	return q.Inverse();
}

Quaternion Quaternion::Conjugate()
{
	return Quaternion( - x ,- y ,- z,w);
}

float Quaternion::Magnitude()
{
	return sqrt(x*x + y*y+z*z+w*w);
}

bool Quaternion::operator==(Quaternion other)
{
	return x == other.x && y == other.y && z == other.z && w == other.w;
}
Quaternion Quaternion::operator/(float f)
{
	return Quaternion(x/f,y/f,z/f,w/f);
}
Quaternion Quaternion::operator*(Quaternion other)
{
	Quaternion a = *this;
	Quaternion b = other;
	Quaternion q = Quaternion(
		a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,   // i
		a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,   // j
		a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,   // k
		a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);  // 1
	
	return q;
}

Quaternion Quaternion::operator*(float f)
{
	return Quaternion(x*f,y*f,z*f,w*f);
}

Quaternion Quaternion::operator+(Quaternion b)
{
	return Quaternion(x + b.x ,y+b.y,z+b.z,w+b.w);
}
