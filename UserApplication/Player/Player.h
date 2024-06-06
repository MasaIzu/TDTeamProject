#pragma once

#include "Sprite3D.h"

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "FBXObject3d.h"

#include<memory>
#include<vector>
#include <Animation.h>
#include "Numbers.h"
#include <ParticleEditor.h>
#include <PlayerMovement.h>

class Enemy;

struct PlayerAnimTime
{
	const uint32_t Step = 30;
	const uint32_t DieMotion = 120;
	const uint32_t BladeAttack = 60;
};

class Player
{
public:

	//コンストラクタとデストラクタ
	Player();
	~Player();

	//初期化
	void Initialize(const unsigned short Attribute,ViewProjection* viewProjection_);
	//更新
	void Update(Input* input);
	//描画
	void Draw(const ViewProjection& LightViewProjection_);

	void Move(Input* input);
	//プレイヤーの回転
	void PlayerRot();
	Vector3 GetPosition() { return worldTransform_.translation_; }

	//パーティクルを出す用
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw();

	//描画
	void FbxDraw(const ViewProjection& lightViewProjection_);
	//描画
	void FbxShadowDraw(const ViewProjection& lightViewProjection_);

	void CheckHitCollision();

	void HpUpdate();
	
	int GetHp() { return hp_; }

	//経験値の増加関数
	void AddExperience(int amount);

	//レベルアップ関数
	void LevelUp();

	//次のレベルアップに必要な経験値を計算(等比数列で計算)
	int CalculateNextLevelExperience() const;

	//プレーヤーの攻撃
	void PlayerBladeAttack();

	//アタックアップデート
	void AttackUpdate();
	//剣の当たり判定属性更新
	void BladeAttributeSet(const unsigned short Attribute_);
	//プレーヤーの移動の値更新
	void WorldTransUpdate();
	int GetLevel() const { return level; }
	int GetExperience() const { return experience; }
	int GetExperienceToNextLevel() const { return experienceToNextLevel; }


	int GetScore() {
		return score_;
	}
	int SetScore(int score = 0) {
		score_ = score;
	}
	void addScore() {
		score_ += addscore_;
	}

private:
	Input* input_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	std::unique_ptr<Model> model_;// 3Dモデル
	std::unique_ptr<FBXObject3d> playerFbx_;
	//移動クラス
	std::unique_ptr<PlayerMovement> playerMovement;
	//アニメーションクラス
	std::unique_ptr<Animation> animation;
	std::unique_ptr<Animation> animation2;
	Vector3 velocity_;
	float speed = 0.7f;


	int hp_ = 100;
	bool isAlive_ = true;
	bool isHit_ = false;

	int score_ = 0;
	const int addscore_ = 1;


	uint32_t RightBoneNum = 34;
	uint32_t BladeAttackEndPos = 39;

	float hitCooltime_ = 5.0f;

#pragma region
	PlayerStateNeedMaterial playerStateNeedMaterial;
	WorldTransform playerRotWorldTrans;

	Vector3 RotKeep;

#pragma endregion
	

#pragma region
	//アニメーションタイム
	PlayerAnimTime playerAnimTime;

	bool isBladeAttack = false;
	bool isBladeAttacking = false;
	bool isPreparation = false;

	uint32_t BladeAttackTime = 30;
	uint32_t BladeMaxAttackTime = 40;

	float BladeColEndHasten = 15.0f;
#pragma endregion

#pragma region
	static const uint32_t AttackColSphereCount = 4;
	const float MaxBladeColDetection = 15.0f;

	std::array<WorldTransform, AttackColSphereCount> BladeColWorldTrans;
	std::array<BaseCollider*, AttackColSphereCount> PlayerBladeAttackCollider;
	std::unique_ptr<ParticleEditor> particleEditor;

	const float PlayerBladeRadius = 2.0f;

	Vector3 BladeColRatio;

	Vector4 ParticleStartPos;
	Vector4 ParticleEndPos;
	Vector4 ParticleMilEndPos;

#pragma endregion

#pragma region
	int level = 1;//レベル
	int experience = 0;//経験値
	int experienceToNextLevel;//次のレベルに必要な経験値
	const int baseExperience = 50;
	const double ratio = 1.2;
#pragma endregion

#pragma region
	unsigned short Attribute_;
	//当たり判定
	BaseCollider* playerCollider;
	CollisionManager* collisionManager = nullptr;
	float playerRadius = 0.5f;
#pragma endregion
};

