#pragma once

#include "Vector4.h"

class BaseCollider;


/// <summary>
/// 衝突情報
/// </summary>
struct CollisionInfo
{
public:
	CollisionInfo(BaseCollider* collider, const Vector4& inter) {
		this->collider = collider;
		this->inter = inter;
	}

	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;
	// 衝突点
	Vector4 inter;
};

