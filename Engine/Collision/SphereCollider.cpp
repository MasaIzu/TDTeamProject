#include "SphereCollider.h"


void SphereCollider::Update(const Matrix4& worldPos)
{
	// ワールド行列から座標を抽出
	worldPos_ = worldPos;

	// 球のメンバ変数を更新
	Vector4 Vector(worldPos_.m[3][0], worldPos_.m[3][1], worldPos_.m[3][2], worldPos_.m[3][3]);
	Sphere::center = Vector;
	Sphere::radius = radius;
}

void SphereCollider::Update(const Matrix4& worldPos, const float& Radius)
{
	// ワールド行列から座標を抽出
	worldPos_ = worldPos;

	// 球のメンバ変数を更新
	Vector4 Vector(worldPos_.m[3][0], worldPos_.m[3][1], worldPos_.m[3][2], worldPos_.m[3][3]);
	Sphere::center = Vector;
	Sphere::radius = Radius;
	Sphere::firstCoolTime = 0;
}

void SphereCollider::Update(const Matrix4& worldPos, const float& Radius,const float& speed, const Vector3& Look)
{
	// ワールド行列から座標を抽出
	worldPos_ = worldPos;

	// 球のメンバ変数を更新
	Vector4 Vector(worldPos_.m[3][0], worldPos_.m[3][1], worldPos_.m[3][2], worldPos_.m[3][3]);
	Sphere::center = Vector;
	Sphere::radius = Radius;
	Sphere::Speed = speed;
	Sphere::look = Look;
	Sphere::firstCoolTime = 0;
}

void SphereCollider::Update(const Matrix4& worldPos, const uint32_t& Cooltime, const bool& isCoolTime)
{

	// ワールド行列から座標を抽出
	worldPos_ = worldPos;

	// 球のメンバ変数を更新
	Vector4 Vector(worldPos_.m[3][0], worldPos_.m[3][1], worldPos_.m[3][2], worldPos_.m[3][3]);
	Sphere::center = Vector;
	Sphere::radius = radius;
	Sphere::coolTime = Cooltime;
	Sphere::firstCoolTime = 0;
	Sphere::isChangeCoolTime = isCoolTime;
}

void SphereCollider::Update(const Matrix4& worldPos, const uint32_t& Cooltime, const uint32_t& FirstCoolTime, const bool& isCoolTime)
{

	// ワールド行列から座標を抽出
	worldPos_ = worldPos;

	// 球のメンバ変数を更新
	Vector4 Vector(worldPos_.m[3][0], worldPos_.m[3][1], worldPos_.m[3][2], worldPos_.m[3][3]);
	Sphere::center = Vector;
	Sphere::radius = radius;
	Sphere::coolTime = Cooltime;
	Sphere::isChangeCoolTime = isCoolTime;
	Sphere::firstCoolTime = FirstCoolTime;
}

void SphereCollider::Update(const Matrix4& worldPos, const float& Radius, const uint32_t& Cooltime)
{
	// ワールド行列から座標を抽出
	worldPos_ = worldPos;

	// 球のメンバ変数を更新
	Vector4 Vector(worldPos_.m[3][0], worldPos_.m[3][1], worldPos_.m[3][2], worldPos_.m[3][3]);
	Sphere::center = Vector;
	Sphere::radius = Radius;
	Sphere::coolTime = Cooltime;
}

void SphereCollider::Update(const Matrix4& worldPos, const float& Radius, const uint32_t& Cooltime, const bool& isCoolTime)
{
	// ワールド行列から座標を抽出
	worldPos_ = worldPos;

	// 球のメンバ変数を更新
	Vector4 Vector(worldPos_.m[3][0], worldPos_.m[3][1], worldPos_.m[3][2], worldPos_.m[3][3]);
	Sphere::center = Vector;
	Sphere::radius = Radius;
	Sphere::coolTime = Cooltime;
	Sphere::isChangeCoolTime = isCoolTime;
}


void SphereCollider::Update(const Matrix4& worldPos, const float& Radius, const uint32_t& Cooltime, const uint32_t& FirstCoolTime, const bool& isCoolTime)
{

	// ワールド行列から座標を抽出
	worldPos_ = worldPos;

	// 球のメンバ変数を更新
	Vector4 Vector(worldPos_.m[3][0], worldPos_.m[3][1], worldPos_.m[3][2], worldPos_.m[3][3]);
	Sphere::center = Vector;
	Sphere::radius = Radius;
	Sphere::coolTime = Cooltime;
	Sphere::isChangeCoolTime = isCoolTime;
	Sphere::firstCoolTime = FirstCoolTime;

}