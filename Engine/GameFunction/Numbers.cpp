#include "Numbers.h"

float FloatNumber(const fNumbers& number)
{
	float Number;
	if ( number != fNumbers::fZero )
	{
		Number = static_cast< float >( number ) / static_cast< float >(fNumbers::fOnePointZero);
	}
	else
	{
		Number = static_cast< float >( number );
	}
    return Number;
}

Vector2 Vec2Number(const fNumbers& number)
{
	float Number = FloatNumber(number);
	return Vector2(Number,Number);
}

Vector2 Vec2Number(const fNumbers& numberX,const fNumbers& numberY)
{
	Vector2 Number;
	Number.x = FloatNumber(numberX);
	Number.y = FloatNumber(numberY);
	return Number;
}

Vector3 Vec3Number(const fNumbers& number)
{
	float Number = FloatNumber(number);
	return Vector3(Number,Number,Number);
}

Vector3 Vec3Number(const fNumbers& numberX,const fNumbers& numberY,const fNumbers& numberZ)
{
	Vector3 Number;
	Number.x = FloatNumber(numberX);
	Number.y = FloatNumber(numberY);
	Number.z = FloatNumber(numberZ);
	return Number;
}

Vector4 Vec4Number(const fNumbers& number)
{
	float Number = FloatNumber(number);
	return Vector4(Number,Number,Number,Number);
}

Vector4 Vec4Number(const fNumbers& numberX,const fNumbers& numberY,const fNumbers& numberZ,const fNumbers& numberW)
{
	Vector4 Number;
	Number.x = FloatNumber(numberX);
	Number.y = FloatNumber(numberY);
	Number.z = FloatNumber(numberZ);
	Number.w = FloatNumber(numberW);
	return Number;
}