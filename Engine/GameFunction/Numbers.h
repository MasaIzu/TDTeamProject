#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

enum class fNumbers
{
	fZero,//0.0f
	fPointOne,//0.1f
	fPointTwo,//0.2f
	fPointThree,//0.3f
	fPointFour,//0.4f
	fPointFive,//0.5f
	fPointSix,//0.6f
	fPointSeven,//0.7f
	fPointEight,//0.8f
	fPointNine,//0.9f
	fOnePointZero,//1.0f
	fOnePointOne,//1.1f
	fOnePointTwo,//1.2f
	fOnePointThree,//1.3f
	fOnePointFour,//1.4f
	fOnePointFive,//1.5f
	fOnePointSix,//1.6f
	fOnePointSeven,//1.7f
	fOnePointEight,//1.8f
	fOnePointNine,//1.9f
	fTwoPointZero,//2.0f
};

enum class Numbers
{
	Zero,//0
	One,//1
	Two,//2
	Three,//3
	Four,//4
	Five,//5
	Six,//6
	Seven,//7
	Eight,//8
	Nine,//9
	Ten//10
};

float FloatNumber(const fNumbers& number);
Vector2 Vec2Number(const fNumbers& number);
Vector2 Vec2Number(const fNumbers& numberX,const fNumbers& numberY);
Vector3 Vec3Number(const fNumbers& number);
Vector3 Vec3Number(const fNumbers& numberX,const fNumbers& numberY,const fNumbers& numberZ);
Vector4 Vec4Number(const fNumbers& number);
Vector4 Vec4Number(const fNumbers& numberX,const fNumbers& numberY,const fNumbers& numberZ,const fNumbers& numberW);