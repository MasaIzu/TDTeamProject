#include "Easing.h"
#include"math.h"
#include <MyMath.h>
//追加

float Easing::In_Back(const float& start,const float& end,const uint32_t& time,const uint32_t& max_time)
{
	float Time = static_cast<float>(time) / static_cast<float>(max_time);
	float move = end - start;
	float c1 = 1.70158f;
	float c3 = ( c1 + 1.0f );
	return start + ( move * ( c3 * Time * Time * Time - c1 * Time * Time ) );
}

float Easing::easeOutCubic(const float& start, const float& end, const uint32_t& time, const uint32_t& max_time)
{
	float Time = static_cast<float>(time) / static_cast<float>(max_time);
	float x = 1 - powf(1 - Time, 3);
	return start + (end - start) * x;
}

float Easing::EaseOutQuart(const float& start, const float& end,const uint32_t& time,const uint32_t& max_time)
{
	float Time = static_cast< float >( time ) / static_cast< float >( max_time );
	float x = 1 - powf(1 - Time, 4);
	return start + (end - start) * x;
}

float Easing::EaseOutQuint(const float& start,const float& end,const float& time,const float& max_time)
{
	float Time = time / max_time;
	float x = 1 - powf(1 - Time,5);
	return start + ( end - start ) * x;
}

float Easing::EaseOutQuint(const float& start,const float& end,const uint32_t& time,const uint32_t& max_time)
{
	float Time = static_cast< float >( time ) / static_cast< float >( max_time );
	float x = 1 - powf(1 - Time,5);
	return start + ( end - start ) * x;
}

float Easing::EaseInSine(const float& start,const float& end,const uint32_t& time,const uint32_t& max_time)
{
	float Time = static_cast< float >( time ) / static_cast< float >( max_time );
	float x = 1.0f - cosf(( Time * MyMath::PI ) / 2.0f);
	return start + ( end - start ) * x;
}

float Easing::EaseInCubic(const float& start,const float& end,const uint32_t& time,const uint32_t& max_time)
{
	float Time = static_cast< float >( time ) / static_cast< float >( max_time );
	float x = Time * Time * Time;
	return start + ( end - start ) * x;
}

float Easing::EaseInQuint(const float& start,const float& end,const uint32_t& time,const uint32_t& max_time)
{
	float Time = static_cast< float >( time ) / static_cast< float >( max_time );
	float x = Time * Time * Time * Time * Time;
	return start + ( end - start ) * x;
}

Vector3 Easing::EaseInBackVec3(const Vector3& start,const Vector3& end,const uint32_t& time,const uint32_t& max_time)
{
	Vector3 easeInBackVec3;

	easeInBackVec3.x = In_Back(start.x,end.x,time,max_time);
	easeInBackVec3.y = In_Back(start.y,end.y,time,max_time);
	easeInBackVec3.z = In_Back(start.z,end.z,time,max_time);

	return easeInBackVec3;
}

Vector3 Easing::EaseOutCubicVec3(const Vector3& start,const Vector3& end,const uint32_t& time,const uint32_t& max_time)
{
	Vector3 easeOutCubicVec3;
	easeOutCubicVec3.x = easeOutCubic(start.x, end.x, time, max_time);
	easeOutCubicVec3.y = easeOutCubic(start.y, end.y, time, max_time);
	easeOutCubicVec3.z = easeOutCubic(start.z, end.z, time, max_time);

	return easeOutCubicVec3;
}

Vector3 Easing::EaseOutQuartVec3(const Vector3& start,const Vector3& end,const uint32_t& time,const uint32_t& max_time)
{
	Vector3 easeOutQuartVec3;

	easeOutQuartVec3.x = EaseOutQuart(start.x,end.x,time,max_time);
	easeOutQuartVec3.y = EaseOutQuart(start.y,end.y,time,max_time);
	easeOutQuartVec3.z = EaseOutQuart(start.z,end.z,time,max_time);

	return easeOutQuartVec3;
}

Vector3 Easing::EaseOutQuintVec3(const Vector3& start,const Vector3& end,const uint32_t& time,const uint32_t& max_time)
{
	Vector3 easeOutQuintVec3;

	float timef = static_cast<float>(time);
	float max_timef = static_cast<float>(max_time);

	easeOutQuintVec3.x = EaseOutQuint(start.x, end.x, timef, max_timef);
	easeOutQuintVec3.y = EaseOutQuint(start.y, end.y, timef, max_timef);
	easeOutQuintVec3.z = EaseOutQuint(start.z, end.z, timef, max_timef);

	return easeOutQuintVec3;
}

Vector3 Easing::EaseInSineVec3(const Vector3& start,const Vector3& end,const uint32_t& time,const uint32_t& max_time)
{
	Vector3 easeInSineVec3;

	easeInSineVec3.x = EaseInSine(start.x,end.x,time,max_time);
	easeInSineVec3.y = EaseInSine(start.y,end.y,time,max_time);
	easeInSineVec3.z = EaseInSine(start.z,end.z,time,max_time);

	return easeInSineVec3;
}
