#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

class PointLight
{

public://サブクラス
	struct ConstBufferData
	{
		Vector3 lightpos;
		float pad1;

		Vector3 lightcolor;
		float pad2;

		Vector3 lightatten;
		unsigned int active;
	};

public:

	inline void SetLightPos(const Vector3& lightpos_) {
		this->lightpos = lightpos_;
	}
	inline const Vector3& GetLightPos() { return lightpos; }
	inline void SetLightColor(const Vector3& lightcolor_) {
		this->lightcolor = lightcolor_;
	}
	inline const Vector3& GetLightColor() { return lightcolor; }
	inline void SetLightAtten(const Vector3& lightAtten_) {
		this->lightAtten = lightAtten_;
	}
	inline const Vector3& GetLightAtten() { return lightAtten; }
	inline void SetActive(bool active_) { this->active = active_; }
	inline bool IsActive() { return active; }



private:

	Vector3 lightpos = { 0,0,0 };
	Vector3 lightcolor = { 1,1,1 };
	Vector3 lightAtten = { 1.0f,1.0f,1.0f };
	bool active = true;

};