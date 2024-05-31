#pragma once

#include "Model.h"
#include "WorldTransform.h"

class Weapon
{
public:

	virtual void Initialize();

	virtual void Update();

	virtual void Draw(ViewProjection ViewProjection_, const ViewProjection& LightViewProjection_);

protected:

	std::unique_ptr<Model> model_;
	WorldTransform worldTransform_;
};

