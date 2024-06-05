#include "EnemyBullet.h"

EnemyBullet::EnemyBullet()
{
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Initialize(const Vector3& position, const Vector3& velocity, Model* bulletModel, ViewProjection* viewProjection_)
{
	model_ .reset(bulletModel) ;


	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//引数で受け取った速度をメンバ関数に代入
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	Vector3 pos = velocity_;
	//座標を移動させる(1フレーム文の移動量を足しこむ)

	worldTransform_.translation_ += velocity_;
	worldTransform_.TransferMatrix();
	deathTimer_--;
	//時間経過でデス
	if (deathTimer_ <= deathTimerEnd_) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& LightViewProjection_)
{
	model_->Draw(worldTransform_, *viewProjection_, LightViewProjection_);
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition()
{
	return worldTransform_.translation_;
}

void EnemyBullet::VecRot()
{
}
