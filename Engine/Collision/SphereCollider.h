#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

/// <summary>
/// 球衝突判定オブジェクト
/// </summary>
class SphereCollider : public BaseCollider, public Sphere
{
public:
	SphereCollider(Vector4 offset = { 0,0,0,0 }, float radius = 1.0f) :
		offset(offset),
		radius(radius)
	{
		// 球形状をセット
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Matrix4& worldPos) override;
	void Update(const Matrix4& worldPos, const float& radius) override;
	void Update(const Matrix4& worldPos, const float& radius,const float& speed,const Vector3& look) override;
	void Update(const Matrix4& worldPos, const uint32_t& Cooltime, const bool& isCoolTime) override;
	void Update(const Matrix4& worldPos, const uint32_t& Cooltime, const uint32_t& FirstCoolTime, const bool& isCoolTime) override;
	void Update(const Matrix4& worldPos, const float& radius, const uint32_t& Cooltime) override;
	void Update(const Matrix4& worldPos, const float& radius, const uint32_t& Cooltime, const bool& isCoolTime) override;
	void Update(const Matrix4& worldPos, const float& radius, const uint32_t& Cooltime, const uint32_t& FirstCoolTime, const bool& isCoolTime) override;

	//半径のセット
	inline void SetRadius(float Radius) { this->radius = Radius; }

	//オフセットのゲット
	inline const Vector4& GetOffset() { return offset; }
	//オフセットのセット
	inline void SetOffset(const Vector4& Offset) { this->offset = Offset; }
	//半径のゲット
	inline float GetRadius() { return radius; }


private:
	// オブジェクト中心からのオフセット
	Vector4 offset;
	// 半径
	float radius;

	//CoolTime
	uint32_t coolTime = 0;

	//isChangeCoolTime
	bool isChangeCoolTime = false;

	bool isHit = false;

};

