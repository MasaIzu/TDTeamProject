#include "EnemyNormalBulletAttack.h"
#include "Enemy.h"
#include "Player.h"

void EnemyNormalBulletAttack::Initialize(Model* bulletModel)
{
	enemyBulletModel_.reset(Model::CreateFromOBJ("cube", true));
}

void EnemyNormalBulletAttack::Update(Enemy* enemy)
{
	enemy_ = enemy;

	EnemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& enemyBullet)
		{
			return enemyBullet->IsDead();
		});

	//’eXV
	for (std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_)
	{
		enemyBullet->Update();
	}
}

void EnemyNormalBulletAttack::Draw(const ViewProjection& LightViewProjection_)
{
	//’eXV
	for (std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_)
	{
		enemyBullet->Draw(LightViewProjection_);
	}
}


void EnemyNormalBulletAttack::Attck(ViewProjection* viewProjection)
{
	velocity_ = player_->GetPosition() - enemy_->GetPosition();
	velocity_.normalize();
	velocity_ *= verocitySpeed;
	//’e‚ğ¶¬‚µA‰Šú‰»
	std::unique_ptr<EnemyBullet> newEnemyBullet = std::make_unique<EnemyBullet>();
	newEnemyBullet->Initialize(enemy_->GetPosition(), velocity_, enemyBulletModel_.get(), viewProjection);
	//’e‚ğ”­Ë‚·‚é
	EnemyBullets_.push_back(std::move(newEnemyBullet));
}
