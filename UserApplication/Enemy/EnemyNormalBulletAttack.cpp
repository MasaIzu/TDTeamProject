#include "EnemyNormalBulletAttack.h"
#include "Enemy.h"
#include "Player.h"

EnemyNormalBulletAttack::EnemyNormalBulletAttack()
{
}

EnemyNormalBulletAttack::~EnemyNormalBulletAttack()
{
	for ( std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_ )
	{
		enemyBullet.release();
	}
}

void EnemyNormalBulletAttack::Initialize(Model* bulletModel)
{
	enemyBulletModel_.reset(Model::CreateFromOBJ("cube",true));
}

void EnemyNormalBulletAttack::Update(Enemy* enemy,Player* player)
{
	enemy_ = enemy;
	player_ = player;

	EnemyBullets_.remove_if([ ] (std::unique_ptr<EnemyBullet>& enemyBullet)
	{
	return enemyBullet->IsDead();
	});
	//弾更新
	for ( std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_ )
	{
		enemyBullet->Update();
	}


}

void EnemyNormalBulletAttack::Draw(const ViewProjection& LightViewProjection_)
{
	//弾更新
	for ( std::unique_ptr<EnemyBullet>& enemyBullet : EnemyBullets_ )
	{
		enemyBullet->Draw(LightViewProjection_);
	}
}


void EnemyNormalBulletAttack::Attck(ViewProjection* viewProjection)
{
	velocity_ = player_->GetPosition() - enemy_->GetPosition();
	velocity_.normalize();
	velocity_ *= verocitySpeed;
		//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newEnemyBullet = std::make_unique<EnemyBullet>();
	newEnemyBullet->Initialize(enemy_->GetPosition(),velocity_,enemyBulletModel_.get(),viewProjection);
	//弾を発射する
	EnemyBullets_.push_back(std::move(newEnemyBullet));


}
