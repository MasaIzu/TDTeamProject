#include "Enemy.h"
#include "Player.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(ViewProjection* viewProjection,Vector3 enemyPos,int actionNmb, Player* player, const unsigned short Attribute)
{

	model_.reset(ModelManager::FindObjModel("cube"));
	modelBullet_.reset(ModelManager::FindObjModel("bullet"));

	worldTransform_.Initialize();
	worldTransform_.translation_ = enemyPos;
	worldTransform_.scale_ = { 10.0f,3.0f,4.0f };
	worldTransform_.TransferMatrix();

	viewProjection_ = viewProjection;

	player_ = player;

	action = actionNmb;

	//enemyNormalBullet = std::make_unique<EnemyNormalBulletAttack>();
	//enemyNormalBullet->Initialize(model_.get());

	// コリジョンマネージャに追加
	float sphereF = 0;
	enemyCollider = new SphereCollider(Vector4(sphereF, enemyRadius, sphereF, sphereF), enemyRadius);
	CollisionManager::GetInstance()->AddCollider(enemyCollider);
	enemyCollider->SetAttribute(COLLISION_ATTR_ALLIES);
	Attribute_ = Attribute;
	enemyCollider->Update(worldTransform_.matWorld_);
	//enemyCollider->Update(animation->GetBonePos(0) * worldTransform_.matWorld_);
}

void Enemy::Update()
{
	if ( attackWaitTime <= 0 )
	{
		BulletAttck();
		attackWaitTime = attackWaitTimeReset;
	}
	//enemyNormalBullet->Update(this,player_);

	Move();
	worldTransform_.TransferMatrix();
	enemyCollider->Update(worldTransform_.matWorld_);
}

void Enemy::Draw(const ViewProjection& LightViewProjection_)
{
	model_->Draw(worldTransform_, *viewProjection_, LightViewProjection_);
	//enemyNormalBullet->Draw(LightViewProjection_);
}

void Enemy::Move()
{
	Vector3 enemyMovement = player_->GetPosition() - worldTransform_.translation_;
	enemyMovement.normalize();
	if ( action == 0 )
	{
		worldTransform_.translation_ += ( enemyMovement * enemySpeed );
	}
	else if ( action == 1 )
	{
		worldTransform_.translation_ -= ( enemyMovement * enemySpeed );
		attackWaitTime--;
	}
	
}

void Enemy::BulletAttck()
{
	//enemyNormalBullet->Attck(viewProjection_);

}

Vector3 Enemy::GetPosition()
{
	return worldTransform_.translation_;
}

