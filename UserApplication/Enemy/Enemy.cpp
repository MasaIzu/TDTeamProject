#include "Enemy.h"
#include "Player.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "ImGuiManager.h"
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(ViewProjection* viewProjection,Vector3 enemyPos,int actionNmb, Player* player, const unsigned short Attribute)
{

	model_.reset(Model::CreateFromOBJ("cube", true));
	debugModel_.reset(Model::CreateFromOBJ("sphere",true));

	worldTransform_.Initialize();
	worldTransform_.translation_ = enemyPos;
	worldTransform_.scale_ = { 7.0f,7.0f,7.0f };
	worldTransform_.TransferMatrix();

	debugTransform.scale_ = { enemyRadius,enemyRadius,enemyRadius };
	debugTransform.Initialize();
	viewProjection_ = viewProjection;

	player_ = player;

	enemyNormalBullet = std::make_unique<EnemyNormalBulletAttack>();
	enemyNormalBullet->Initialize(model_.get());

	// コリジョンマネージャに追加
	float sphereF = 0;
	enemyCollider = new SphereCollider(Vector4(sphereF, enemyRadius, sphereF, sphereF), enemyRadius);
	CollisionManager::GetInstance()->AddCollider(enemyCollider);
	enemyCollider->SetAttribute(COLLISION_ATTR_ENEMYS);
	Attribute_ = Attribute;
	enemyCollider->Update(worldTransform_.matWorld_);
	//enemyCollider->Update(animation->GetBonePos(0) * worldTransform_.matWorld_);
}

void Enemy::Update()
{



	Move();
	//enemyNormalBullet->Update(this);
	worldTransform_.TransferMatrix();
	enemyCollider->Update(worldTransform_.matWorld_);
	debugTransform.translation_ = worldTransform_.translation_;
	debugTransform.TransferMatrix();

	if ( enemyCollider->GetMeleeHit() )
	{
		livingTimer_ = 0;
		player_->addScore();
		enemyCollider->ResetMeleeHit();
		CollisionManager::GetInstance()->RemoveCollider(enemyCollider);

	}
	if ( enemyCollider->GetHit() )
	{
		player_->addScore();
	}
	livingTimer_--;

	if ( livingTimer_ <= 0 )
	{
		isDead_ = true;
		livingTimer_ = 300;
	}
	//if ( isDead_ == true )
	//{

	//}




}

void Enemy::Draw(const ViewProjection& LightViewProjection_)
{
	//model_->Draw(worldTransform_, *viewProjection_, LightViewProjection_);
	//enemyNormalBullet->Draw(LightViewProjection_);
	debugModel_->Draw(debugTransform,*viewProjection_,LightViewProjection_);
}

void Enemy::Move()
{
	Vector3 enemyMovement = player_->GetPosition() - worldTransform_.translation_;
	enemyMovement.normalize();
	worldTransform_.translation_ += (enemyMovement*enemySpeed);

	

}

void Enemy::BulletAttck()
{
	//enemyNormalBullet->Attck(viewProjection_);
}

Vector3 Enemy::GetPosition()
{
	return worldTransform_.translation_;
}

