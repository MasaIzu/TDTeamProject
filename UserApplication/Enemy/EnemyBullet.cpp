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


	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
	//�����Ŏ󂯎�������x�������o�֐��ɑ��
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	Vector3 pos = velocity_;
	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)

	worldTransform_.translation_ += velocity_;
	worldTransform_.TransferMatrix();
	deathTimer_--;
	//���Ԍo�߂Ńf�X
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
