#pragma once
#include "UserApplication/Weapon/Weapon.h"

class PlayerSword : public Weapon
{
public:

	void Initialize()override;

	void Update()override;

	void Draw(ViewProjection ViewProjection_, const ViewProjection& LightViewProjection_)override;

	WorldTransform GetWorldTransform() { return worldTransform_; }

	void SetPosition(Vector3 pos) { worldTransform_.translation_ = pos; }

	const WorldTransform* GetParent() { return worldTransform_.parent_; }

	void SetParent(const WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }

protected:

};
