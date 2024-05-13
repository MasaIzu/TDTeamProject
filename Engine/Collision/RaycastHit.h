#pragma once

#include "BaseCollider.h"

#include"Vector4.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include<DirectXMath.h>
MY_SUPPRESS_WARNINGS_END
//レイキャストプリミティブ
struct RaycastHit {

	BaseCollider* collider = nullptr;

	Vector4 inter;

	float distance = 0.0f;

};
