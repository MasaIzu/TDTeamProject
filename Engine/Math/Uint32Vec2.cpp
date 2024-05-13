#include "Uint32Vector2.h"
#include<cmath>  //sprt

Uint32Vector2::Uint32Vector2() :x(0), y(0)
{
}

Uint32Vector2::Uint32Vector2(const uint32_t& x,const uint32_t& y) {
	this->x = x;
	this->y = y;
}

Uint32Vector2& Uint32Vector2::operator+=(const Uint32Vector2& v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

Uint32Vector2& Uint32Vector2::operator-=(const Uint32Vector2& v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

Uint32Vector2& Uint32Vector2::operator*=(uint32_t s)
{
	this->x *= s;
	this->y *= s;
	return *this;
}

Uint32Vector2& Uint32Vector2::operator/=(uint32_t s)
{
	x /= s;
	y /= s;
	return *this;
}

//Vector2 クラスに属さない関数群
const Uint32Vector2 operator+(const Uint32Vector2& v1, const Uint32Vector2& v2)
{
	Uint32Vector2 temp = v1;
	Uint32Vector2 temp2 = v2;
	temp.x = temp.x + temp2.x;
	temp.y = temp.y + temp2.y;

	return temp;
}

const Uint32Vector2 operator-(const Uint32Vector2& v1, const Uint32Vector2& v2)
{
	Uint32Vector2 temp = v1;
	Uint32Vector2 temp2 = v2;
	temp.x = temp.x - temp2.x;
	temp.y = temp.y - temp2.y;

	return temp;
}

const Uint32Vector2 operator*(const Uint32Vector2& v, uint32_t s)
{
	Uint32Vector2 temp;
	temp.x = v.x * s;
	temp.y = v.y * s;

	return temp;
}

const Uint32Vector2 operator*(uint32_t s, const Uint32Vector2& v)
{
	Uint32Vector2 temp;
	temp.x = v.x * s;
	temp.y = v.y * s;

	return temp;
}

const Uint32Vector2 operator/(const Uint32Vector2& v, uint32_t s)
{
	Uint32Vector2 temp;
	temp.x = v.x / s;
	temp.y = v.y / s;

	return temp;
}