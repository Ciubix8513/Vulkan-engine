#include "Vector3.h"

Vector3::Vector3(float NewX, float NewY, float NewZ)
{
	x = NewX; 
	y = NewY;
	z = NewZ;
}
Vector3::Vector3(Vector2 NewXY, float NewZ)
{
	x = NewXY.x;
	y = NewXY.y;
	z = NewZ;
}
Vector3::Vector3(float NewX, Vector2 NewYZ)
{
	x = NewX;
	y = NewYZ.x;
	z = NewYZ.y;

}
EngineMath::Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}
Vector3 EngineMath::Vector3::Up()
{
	return Vector3(0,1,0);
}
Vector3 EngineMath::Vector3::Down()
{
	return Vector3(0,-1,0);
}
Vector3 EngineMath::Vector3::Forward()
{
	return Vector3(0,0,1);
}
Vector3 EngineMath::Vector3::Backward()
{
	return Vector3(0,0,-1);
}
Vector3 EngineMath::Vector3::Right()
{
	return Vector3(1,0,0);
}
Vector3 EngineMath::Vector3::Left()
{
	return Vector3(-1,0,0);
}
Vector3 EngineMath::Vector3::Zero()
{
	return Vector3(0,0,0);
}

float EngineMath::Vector3::Length()
{
	return (float)sqrt(((double)x) * ((double)x)+ ((double)y)* ((double)y)+ ((double)z)* ((double)z));
}
float EngineMath::Vector3::Length(Vector3& v)
{
	return v.Length();
}

EngineMath::Vector3 EngineMath::Vector3::Normalized()
{
	return Vector3(x,y,z)  / Length();
}

void EngineMath::Vector3::Normalize()
{
	x /= Length();
	y /= Length();
	z /= Length();
	return;
}

float EngineMath::Vector3::DotProduct(Vector3 a, Vector3 b)
{
	return a.x*b.x+ a.y * b.y+ a.z * b.z;

}
Vector3 EngineMath::Vector3::CrossProdut(Vector3 a, Vector3 b)
{
	return Vector3(a.y*b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}




Vector3 EngineMath::Vector3::operator+(Vector3 v)
{
	return Vector3(x + v.x,y + v.y,z + v.z);
}
Vector3 EngineMath::Vector3::operator-(Vector3 v)
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}
Vector3 EngineMath::Vector3::operator-(float c)
{
	return Vector3(x -c,y-c,z-c);
}
Vector3 EngineMath::Vector3::operator*(float c)
{
	return Vector3(x*c,y*c,z*c);
}
float EngineMath::Vector3::operator*(Vector3 c)
{
	return DotProduct(Vector3(x,y,z),c);
}
Vector3 EngineMath::Vector3::operator/(float c)
{
	return Vector3(x / c, y / c, z / c);
}

void EngineMath::Vector3::operator+=(Vector3 v)
{
	x = x + v.x;
	y = y + v.y;
	z = z + v.z;
	return;
}
void EngineMath::Vector3::operator-=(Vector3 v)
{
	x = x - v.x;
	y = y - v.y;
	z = z - v.z;
	return;
}
void EngineMath::Vector3::operator*=(float c)
{
	x = x * c;
	y = y * c;
	z = z * c;
	return;
}
void EngineMath::Vector3::operator/=(float c)
{
	x = x / c;
	y = y / c;
	z = z / c;
	return;
}

EngineMath::Vector3::operator DirectX::XMFLOAT3()
{
	return DirectX::XMFLOAT3(x, y, z);
}


bool EngineMath::Vector3::operator!=(Vector3 v)
{
	if(x == v.x && z == v.z && y == v.y)
	return false;	
	return true;
}
