#include "PlayerSword.h"

void PlayerSword::Initialize()
{
	model_.reset(Model::CreateFromOBJ("cube", true));
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,30,50 };
	worldTransform_.scale_ = { 10.0f,3.0f,4.0f };
	worldTransform_.TransferMatrix();
}

void PlayerSword::Update()
{
	worldTransform_.TransferMatrix();
}

void PlayerSword::Draw(ViewProjection ViewProjection_,const ViewProjection& LightViewProjection_)
{
	model_->Draw(worldTransform_, ViewProjection_, LightViewProjection_);
}
