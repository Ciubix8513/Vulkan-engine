#include "Vector2.h"

EngineMath::Vector2::Vector2()
{
	x = 0;
	y = 0;
}

EngineMath::Vector2::Vector2(float NewX, float NewY)
{
	x = NewX;
	y = NewY;
	
}

EngineMath::Vector2 EngineMath::Vector2::Up()
{
	return Vector2(0, 1);
}
EngineMath::Vector2 EngineMath::Vector2::Down()
{
	return Vector2(0, -1);
}
EngineMath::Vector2 EngineMath::Vector2::Right()
{
	return Vector2(1, 0);
}
EngineMath::Vector2 EngineMath::Vector2::Left()
{
	return Vector2(-1, 0);
}
EngineMath::Vector2 EngineMath::Vector2::Zero()
{
	return Vector2(0, 0);
}

float EngineMath::Vector2::Length()
{
	return (float)sqrt((float)(x * x + y * y));
}
float EngineMath::Vector2::Length(Vector2& v)
{
	return v.Length();
}

EngineMath::Vector2 EngineMath::Vector2::Normalized()
{
	return Vector2(x, y) / Length();
}
void EngineMath::Vector2::Normalize(Vector2& v)
{
	v /= v.Length();
	return;
}
void EngineMath::Vector2::Normalize()
{
	x /= Length();
	y /= Length();
	
	return;
}

float EngineMath::Vector2::DotProduct(Vector2 a, Vector2 b)
{
	return a.x * b.x + a.y * b.y ;
}




EngineMath::Vector2 EngineMath::Vector2::operator+(Vector2 v)
{
	return Vector2(x + v.x, y + v.y);
}
EngineMath::Vector2 EngineMath::Vector2::operator-(Vector2 v)
{
	return Vector2(x - v.x, y - v.y);
}
EngineMath::Vector2 EngineMath::Vector2::operator*(float c)
{
	return Vector2(x * c, y * c);
}
EngineMath::Vector2 EngineMath::Vector2::operator/(float c)
{
	return Vector2(x / c, y / c);
}

void EngineMath::Vector2::operator+=(Vector2 v)
{
	x = x + v.x;
	y = y + v.y;

	return;
}
void EngineMath::Vector2::operator-=(Vector2 v)
{
	x = x - v.x;
	y = y - v.y;

	return;
}
void EngineMath::Vector2::operator*=(float c)
{
	x = x * c;
	y = y * c;

	return;
}
void EngineMath::Vector2::operator/=(float c)
{
	x = x / c;
	y = y / c;

	return;
}


