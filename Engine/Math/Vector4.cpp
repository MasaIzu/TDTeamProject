#include "Vector4.h"

#include<cmath>  //sprt
#include <Vector3.h>

Vector4::Vector4() :x(0), y(0), z(0), w(0)
{
}


Vector4::Vector4(const float& x,const float& y,const float& z,const float& w){
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

float Vector4::length()const {
	return std::sqrt(x * x + y * y + z * z + w * w);
}

Vector4& Vector4::normalize() {
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

float Vector4::dot(const Vector4& v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

float Vector4::Vector3Dot(const Vector4& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector4 Vector4::cross(const Vector4& v) const
{
	return Vector4(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x,
		v.w
	);
}

float Vector4::Vector3Length() const
{
	float len = Vector3LengthSquared();
	return sqrt(len);
}

float Vector4::Vector3LengthSquared() const
{
	return Vector3Dot(*this);
}

Vector4 Vector4::Vector3Normalization() const
{
	Vector4 tmp(*this);

	float len = Vector3Length();
	if (len != 0)
	{
		return tmp /= len;

	}

	return tmp;
}

Vector4& Vector4::Vector3Normal()
{
	float len = Vector3Length();
	if (len != 0)
	{
		return *this /= len;

	}

	return *this;
}

Vector4 Vector4::Vector3Cross(const Vector4& v) const
{
	Vector3 v1 = { x,y,z };
	Vector3 v2 = { v.x,v.y,v.z };
	Vector3 temp = v1.cross(v2);
	return { temp.x,temp.y,temp.z,w };
}


Vector4 Vector4::operator+() const
{
	return Vector4(x, y, z, w);
}

Vector4 Vector4::operator-() const
{
	return Vector4(x, y, z, w);
}

Vector4& Vector4::operator+=(const Vector4& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	this->w += v.w;
	return *this;
}

Vector4& Vector4::operator-=(const Vector4& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	this->w -= v.w;
	return *this;
}

Vector4& Vector4::operator*=(float s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	this->w *= s;
	return *this;
}

Vector4& Vector4::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

Vector4& Vector4::operator=(float v[ 4 ])
{
	x = v[ 0 ];
	y = v[ 1 ];
	z = v[ 2 ];
	w = v[ 3 ];
	return *this;
}

//Vector4 クラスに属さない関数群


const Vector4 operator+(const Vector4& v1, const Vector4& v2)
{
	Vector4 temp(v1);

	temp.x += v2.x;
	temp.y += v2.y;
	temp.z += v2.z;
	temp.w += v2.w;

	return temp;
}

const Vector4 operator-(const Vector4& v1, const Vector4& v2)
{
	Vector4 temp(v1);

	temp.x -= v2.x;
	temp.y -= v2.y;
	temp.z -= v2.z;
	temp.w -= v2.w;

	return temp;
}

const Vector4 operator*(const Vector4& v, float s)
{
	Vector4 temp(v);

	temp.x *= s;
	temp.y *= s;
	temp.z *= s;
	temp.w *= s;

	return temp;
}

const Vector4 operator*(float s, const Vector4& v)
{
	return v * s;
}

const Vector4 operator/(const Vector4& v, float s)
{
	Vector4 temp(v);

	temp.x /= s;
	temp.y /= s;
	temp.z /= s;
	temp.w /= s;

	return temp;
}

Vector4 Vector4Lerp(const Vector4& src1, const Vector4& src2, float t)
{
	Vector4 temp;
	Vector4Lerp(src1, src2, t, temp);
	return temp;
}

void Vector4Lerp(const Vector4& src1, const Vector4& src2, float t, Vector4& dest)
{
	dest.x = src1.x + (src2.x - src1.x) * t;
	dest.y = src1.y + (src2.y - src1.y) * t;
	dest.z = src1.z + (src2.z - src1.z) * t;
	dest.w = src1.w + (src2.w - src1.w) * t;
}