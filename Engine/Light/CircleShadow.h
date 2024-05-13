#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

/// <summary>
/// 丸影
/// </summary>
class CircleShadow {

public:
	//サブクラス
	struct ConstBufferData {
		Vector4 dir;
		Vector3 casterPos;
		float distanceCasterLight;
		Vector3 atten;
		float pad3;
		Vector2 factorAngleCos;
		unsigned int active;
		float pad4;
	};
	
public:
	//メンバ関数

	inline void SetDir(const Vector4& dir_) { this->dir = dir_.Vector3Normalization(); }
	inline const Vector4& GetDir() { return dir; }

	inline void SetCasterPos(const Vector3& casterPos_) { this->casterPos = casterPos_; }
	inline const Vector3& GetCasterPos() { return casterPos; }

	inline void SetDistanceCasterLight(const float& distanceCasterLight_) { this->distanceCasterLight = distanceCasterLight_; }
	inline float GetDistanceCasterLight() { return distanceCasterLight; }
	
	inline void SetAtten(const Vector3& atten_) { this->atten = atten_; }
	inline const Vector3& GetAtten() { return atten; }

	inline void SetFactorAngle(const Vector2& factorAngle_) {
		this->factorAngleCos.x = cosf(DirectX::XMConvertToRadians(factorAngle_.x));
		this->factorAngleCos.y = cosf(DirectX::XMConvertToRadians(factorAngle_.y));
	}

	inline const Vector2& GetFactorAngleCos() { return factorAngleCos; }
	inline void SetActive(const bool& active_) { this->active = active_; }
	inline bool IsActive() { return active; }

private:
	//方向
	Vector4 dir = { 1,0,0,0 };

	float distanceCasterLight = 100.0f;

	Vector3 casterPos = { 0,0,0 };

	Vector3 atten = { 0.5f,0.6f,0.0f };

	Vector2 factorAngleCos = { 0.2f,0.5f };

	bool active = false;

};