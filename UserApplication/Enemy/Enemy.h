#pragma once

#pragma once

#include "Sprite3D.h"

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"
#include "FBXObject3d.h"

#include "CollisionManager.h"
#include "BaseCollider.h"

#include "Vector4.h"

#include "EnemyBullet.h"
#include "EnemyNormalBulletAttack.h"

#include<memory>
#include<vector>
#include <Animation.h>

#include "ParticleEditor.h"

class Player;

class Enemy
{
public:

	//コンストラクタとデストラクタ
	Enemy();
	~Enemy();

	//初期化
	void Initialize(ViewProjection* viewProjection_,Model* model,Vector3 enemyPos,int actionNmb,Player* player,const unsigned short Attribute,int power,int hp);
	//更新
	void Update();

	//パーティクルを出す用
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw();

	//描画
	void Draw(const ViewProjection& LightViewProjection_);
	//移動
	void Move();

	void Damage();

	void SkillDamage();

	//経験値のセッター
	void SetExp(int experience) {
		experience_ = experience;
	}


//死亡判定
	bool IsDead()const {
		return isDead_;
	}

	Player* GetPlayer() {
		return player_;
	}
	void SetPlayer(Player* player) {
		player_ = player_;
	}

	void BulletAttck();

	Vector3 GetPosition();

	void SetHp(int hp) {hp_ = hp;}//体力のセッター
	int GetHp(){return hp_;}//体力のゲッター



private:

	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	Model* model_;// 3Dモデル
	Model* sakabanModel_;// 3Dモデル
	Player* player_;

	std::unique_ptr<ParticleEditor> deadParticleEditor_;

	std::list<std::unique_ptr<EnemyBullet>> EnemyBullets_;
	std::unique_ptr<EnemyNormalBulletAttack> enemyNormalBullet;

	Vector3 velocity_;
	float speed = 0.1f;
	int experience_;//経験値

	//デスフラグ
	bool isDead_ = false;

	float enemySpeed = 0.3f;
	int livingTimer_ = 300;//生存時間

	int hp_;//体力
	int power_;//攻撃力
	int damage_;//被ダメージ量

	int skillDamage_;//スキル攻撃による被ダメージ量

	bool isHitStop = false;
	float hitStopTimer_ = 0;
	const float MAX_HITSTOP = 30;

	float hitDalay = false;
#pragma region

	bool isHit_ = false;
	unsigned short Attribute_;
	//当たり判定
	BaseCollider* enemyCollider;
	CollisionManager* collisionManager = nullptr;
	float enemyRadius = 7.0f;

	WorldTransform debugTransform;
	std::unique_ptr<Model> debugModel_;

	Vector4 ParticleStartPos;
	Vector4 ParticleEndPos;

#pragma endregion
	float Angle;
};





