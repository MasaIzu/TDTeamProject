#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(ViewProjection* viewProjection)
{

	model_.reset(Model::CreateFromOBJ("cube", true));


	worldTransform_.Initialize();
	worldTransform_.translation_ = { 50,0,0 };
	worldTransform_.scale_ = { 10.0f,3.0f,4.0f };
	worldTransform_.TransferMatrix();

	viewProjection_ = viewProjection;
}

void Enemy::Update()
{
	Move();
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(const ViewProjection& LightViewProjection_)
{
	model_->Draw(worldTransform_, *viewProjection_, LightViewProjection_);
}

void Enemy::Move()
{
}

