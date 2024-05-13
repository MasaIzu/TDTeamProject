#include "Vector3.h"

#include<cmath>  //sprt

Vector3::Vector3() :x(0), y(0), z(0)
{
}

Vector3::Vector3(const float& x,const float& y,const float& z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3::length()const {
	return std::sqrt(x * x + y * y + z * z);
}

Vector3& Vector3::normalize() {
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

Vector3 Vector3::norm()const
{
	Vector3 nor = *this;

	float len = length();
	if (len != 0) {
		return nor /= len;
	}
	
	return nor;
}

float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::cross(const Vector3& v) const
{
	return Vector3(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}

Vector3 Vector3::faceNormal(const Vector3& v1,const Vector3& v2,const Vector3& v3)
{
	Vector3 n;

	// 頂点を結ぶベクトルを算出
	Vector3 vec1 = { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
	Vector3 vec2 = { v3.x - v1.x, v3.y - v1.y, v3.z - v1.z };

	// ベクトル同士の外積
	n.x = vec1.y * vec2.z - vec1.z * vec2.y;
	n.y = vec1.z * vec2.x - vec1.x * vec2.z;
	n.z = vec1.x * vec2.y - vec1.y * vec2.x;

	return n;
}

//線形補間
const Vector3 Vector3::lerp(const Vector3& start, const Vector3& end, const float t) {
	
	return start * (1.0f - t) + end * t;
}

Vector3 Vector3::operator+()const {
	return *this;
}
Vector3 Vector3::operator-()const {
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}

Vector3& Vector3::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Vector3& Vector3::operator=(float s[ 3 ])
{
	x = s[0];
	y = s[1];
	z = s[2];
	return *this;
}

//Vector3 クラスに属さない関数群


const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	temp += v2;
	return temp;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	temp -= v2;
	return temp;
}

const Vector3 operator*(const Vector3& v, float s){
	Vector3 temp = v;
	temp.x = temp.x * s;
	temp.y = temp.y * s;
	temp.z = temp.z * s;

	return temp;
}

const Vector3 operator*(float s, const Vector3& v)
{
	Vector3 temp = v;
	temp.x = temp.x * s;
	temp.y = temp.y * s;
	temp.z = temp.z * s;

	return temp;
}

const Vector3 operator*(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp = v1;
	temp *= v2;
	return temp;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp = v;
	temp.x = temp.x / s;
	temp.y = temp.y / s;
	temp.z = temp.z / s;

	return temp;
}