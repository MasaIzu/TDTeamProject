#pragma once
#include "Vector3.h"


#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <cstdint>
MY_SUPPRESS_WARNINGS_END

//イージング
namespace Easing
{
	//動きと詳細はイージングチートシートを見て確認してくれ

	float In_Back(const float& start,const float& end,const uint32_t& time,const uint32_t& max_time);

	float easeOutCubic(const float& start,const float& end,const uint32_t& time,const uint32_t& max_time);
	float EaseOutQuart(const float& start,const float& end,const uint32_t& time,const uint32_t& max_time);
	float EaseOutQuint(const float& start,const float& end,const float& time,const float& max_time);
	float EaseOutQuint(const float& start,const float& end,const uint32_t& time,const uint32_t& max_time);
	float EaseInSine(const float& start,const float& end,const uint32_t& time,const uint32_t& max_time);
	float EaseInCubic(const float& start,const float& end,const uint32_t& time,const uint32_t& max_time);
	float EaseInQuint(const float& start,const float& end,const uint32_t& time,const uint32_t& max_time);

	//Vector3にしただけ
	Vector3 EaseInBackVec3(const Vector3& start,const Vector3& end,const uint32_t& time,const uint32_t& max_time);
	Vector3 EaseOutCubicVec3(const Vector3& start,const Vector3& end,const uint32_t& time,const uint32_t& max_time);
	Vector3 EaseOutQuartVec3(const Vector3& start,const Vector3& end,const uint32_t& time,const uint32_t& max_time);
	Vector3 EaseOutQuintVec3(const Vector3& start,const Vector3& end,const uint32_t& time,const uint32_t& max_time);
	Vector3 EaseInSineVec3(const Vector3& start,const Vector3& end,const uint32_t& time,const uint32_t& max_time);
};
