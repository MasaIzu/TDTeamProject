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

void Enemy::Initialize(ViewProjection* viewProjection,Model* model,Vector3 enemyPos,int actionNmb, Player* player, const unsigned short Attribute,int power,int hp)
{

	sakabanModel_ = model;
	//debugModel_.reset(Model::CreateFromOBJ("sphere",true));

	worldTransform_.Initialize();
	worldTransform_.translation_ = enemyPos;
	worldTransform_.scale_ = { 7.0f,7.0f,7.0f };
	worldTransform_.TransferMatrix();

	debugTransform.scale_ = { enemyRadius,enemyRadius,enemyRadius };
	debugTransform.Initialize();
	viewProjection_ = viewProjection;

	player_ = player;

	enemyNormalBullet = std::make_unique<EnemyNormalBulletAttack>();
	enemyNormalBullet->Initialize(model_);

	int MaxParticleCountB = 1000;
	deadParticleEditor_ = std::make_unique<ParticleEditor>();
	deadParticleEditor_->Initialize(MaxParticleCountB,true,"HitE2");
	deadParticleEditor_->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));


	NormalHitParticleEditor_ = std::make_unique<ParticleEditor>();
	NormalHitParticleEditor_->Initialize("EnemyNormalHit");
	NormalHitParticleEditor_->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));


	// コリジョンマネージャに追加
	float sphereF = 0;
	enemyCollider = new SphereCollider(Vector4(sphereF, enemyRadius, sphereF, sphereF), enemyRadius);
	CollisionManager::GetInstance()->AddCollider(enemyCollider);
	enemyCollider->SetAttribute(COLLISION_ATTR_ENEMYS);
	Attribute_ = Attribute;
	enemyCollider->Update(worldTransform_.matWorld_);
	//enemyCollider->Update(animation->GetBonePos(0) * worldTransform_.matWorld_);

	power_ = power;
	hp_ = hp;

}

void Enemy::Update()
{


	ParticleStartPos = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(worldTransform_.matWorld_));
	//ParticleEndPos = MyMath::Vec3ToVec4(MyMath::GetWorldTransform((worldTransform_.matWorld_)));


	//BladeColRatio = MyMath::Vec4ToVec3(ParticleEndPos) - MyMath::Vec4ToVec3(ParticleStartPos);
	//ParticleMilEndPos = ParticleStartPos + MyMath::Vec3ToVec4(BladeColRatio.norm() * MaxBladeColDetection);
	//BladeColRatio = ( BladeColRatio.norm() * MaxBladeColDetection ) / AttackColSphereCount;


	if ( isHitStop == false )
	{
		Move();
	}

	//enemyNormalBullet->Update(this);
	worldTransform_.TransferMatrix();
	enemyCollider->Update(worldTransform_.matWorld_);
	//debugTransform.translation_ = worldTransform_.translation_;
	//debugTransform.TransferMatrix();

	if ( enemyCollider->GetMeleeHit() )
	{
		//livingTimer_ = 0;
		Damage();
		isHitStop = true;
		enemyCollider->ResetMeleeHit();
	}

	//プレイヤーのスキル攻撃と当たったときの処理
	if ( enemyCollider->GetIsPlayerSkillAttackHit() )
	{
		//livingTimer_ = 0;
		SkillDamage();
		isHitStop = true;
		enemyCollider ->PlayerSkillAttackHitReset();
	}

	//被弾時にヒットストップを入れる
	if ( isHitStop == true )
	{
		hitStopTimer_++;
	}
	if ( hitStopTimer_ > MAX_HITSTOP )//ある程度経ったらヒットストップを解除する
	{
		hitStopTimer_ = 0;
		isHitStop = false;
	}

	if ( enemyCollider->GetHit() )
	{
		player_->addScore();
	}
	livingTimer_--;

	if ( livingTimer_ <= 0 )
	{
		isDead_ = true;
		livingTimer_ = 480;
		CollisionManager::GetInstance()->RemoveCollider(enemyCollider);
	}

	if ( hp_ <= 0 )
	{
		hp_ = 0;
		isDead_ = true;
		player_->addScore();
	}

}

void Enemy::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{
	deadParticleEditor_->CSUpdate(cmdList,ParticleStartPos,isDead_);
	NormalHitParticleEditor_->CSUpdate(cmdList,ParticleStartPos,isHitStop);
}

void Enemy::ParticleDraw()
{
	deadParticleEditor_->Draw(*viewProjection_);
	NormalHitParticleEditor_->Draw(*viewProjection_);
}

void Enemy::Draw(const ViewProjection& LightViewProjection_)
{
	sakabanModel_->Draw(worldTransform_, *viewProjection_, LightViewProjection_);
	//enemyNormalBullet->Draw(LightViewProjection_);
	//debugModel_->Draw(debugTransform,*viewProjection_,LightViewProjection_);
}

void Enemy::Move()
{
	Vector3 enemyMovement = player_->GetPosition() - worldTransform_.translation_;
	enemyMovement.normalize();
	worldTransform_.translation_ += (enemyMovement*enemySpeed);

	
	Angle = MyMath::Get2VecAngle(worldTransform_.translation_ + worldTransform_.LookVelocity.look,player_->GetPosition());

	worldTransform_.SetRot(Vector3(FloatNumber(fNumbers::fZero),MyMath::GetAngle(Angle),FloatNumber(fNumbers::fZero)));
}

void Enemy::Damage()
{

	damage_ = player_->GetPower();
	hp_ -= damage_;

}

void Enemy::SkillDamage()
{
	skillDamage_ = player_->GetSkillPower();
	hp_ -= skillDamage_;

}

void Enemy::BulletAttck()
{
	//enemyNormalBullet->Attck(viewProjection_);
}

Vector3 Enemy::GetPosition()
{
	return worldTransform_.translation_;
}

