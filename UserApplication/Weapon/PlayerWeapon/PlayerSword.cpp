#include "PlayerSword.h"

void PlayerSword::Initialize()
{
	//e‚Ì’l‚©‚ç‚Ç‚ê‚¾‚¯ˆÚ“®AŠg‘åA‰ñ“]‚µ‚½‚©
	model_.reset(Model::CreateFromOBJ("cube", true));
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,10,0 };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.TransferMatrix();
}

void PlayerSword::Update()
{
	worldTransform_.TransferMatrix();
}

void PlayerSword::Draw(ViewProjection ViewProjection_, const ViewProjection& LightViewProjection_)
{
	model_->Draw(worldTransform_, ViewProjection_, LightViewProjection_);
}
