#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

/// <summary>
/// スポットライト
/// </summary>
class SpotLight {

public://サブクラス
	struct ConstBufferData
	{
		Vector4 lightv;
		Vector3 lightpos;
		float pad1;
		Vector3 lightcolor;
		float pad2;
		Vector3 lightatten;
		float pad3;
		Vector2 lightfactoranglecos;
		unsigned int active;
		float pad4;
		
	};

public:
	//メンバ関数
	inline void SetLightDir(const Vector4& lightdir_) { this->lightdir = lightdir_.Vector3Normalization(); }
	inline const Vector4& GetLightDir() { return lightdir; }
	inline void SetLightPos(const Vector3& lightpos_) { this->lightpos = lightpos_; }
	inline const Vector3& GetLightPos() { return lightpos; }
	inline void SetLightColor(const Vector3& lightcolor_) { this->lightcolor = lightcolor_; }
	inline const Vector3& GetLightColor() { return lightcolor; }
	inline void SetLightAtten(const Vector3& lightAtten_) { this->lightAtten = lightAtten_; }
	inline const Vector3& GetLightAtten() { return lightAtten; }
	inline void SetLightFactorAngle(const Vector2& lightFactorAngle_) {
		this->lightFactorAngleCos.x = cosf(DirectX::XMConvertToRadians(lightFactorAngle_.x));
		this->lightFactorAngleCos.y = cosf(DirectX::XMConvertToRadians(lightFactorAngle_.y));
	}
	inline const Vector2& GetLightFactorAngleCos() { return lightFactorAngleCos; }
	inline void SetActive(bool active_) { this->active = active_; }
	inline bool IsActive() { return active; }


private:

	//ライトの方向
	Vector4 lightdir = { 1,0,0,0 };
	//ライト座標
	Vector3 lightpos = { 0,0,0 };
	//ライトの色
	Vector3 lightcolor = { 1,1,1 };
	//ライトの距離減衰係数
	Vector3 lightAtten = { 1.0f,1.0f,1.0f };
	//ライト減衰角度
	Vector2 lightFactorAngleCos = { 0.5f,0.2f };
	//有効フラグ
	bool active = true;


};