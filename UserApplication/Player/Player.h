#pragma once

#include "Sprite3D.h"
#include "Sprite.h"
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
#include <Trail3D.h>

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

	//トレイル描画
	void TarilDraw();

	void Move(Input* input);
	//プレイヤーの回転
	void PlayerRot();
	Vector3 GetPosition() { return worldTransform_.translation_; }

	//パーティクルを出す用
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw();


	void SpriteUpdate();

	//描画
	void FbxDraw(const ViewProjection& lightViewProjection_);
	//描画
	void FbxShadowDraw(const ViewProjection& lightViewProjection_);

	void SpriteDraw();

	void CheckHitCollision();

	void HpUpdate();
	


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

	//落雷の当たり判定属性更新
	void SunderAttributeSet(const unsigned short Attribute_);

	//プレーヤーの移動の値更新
	void WorldTransUpdate();
	int GetLevel() const { return level; }
	int GetExperience() const { return experience; }
	int GetExperienceToNextLevel() const { return experienceToNextLevel; }
	void GetEnemyPos(const Vector3& enemyPos);

	
	///setterとgetter
	//スコア
	int GetScore() {
		return score_;
	}
	int SetScore(int score = 0) {
		score_ = score;
	}
	void addScore() {
		score_ += addscore_;
	}
	//攻撃力
	int GetPower() {
		return power_;
	}
	void SetPower(int power) {
		power_ = power;
	}
	//体力
	int GetHp() {
		return hp_;
	}
	void SetHp(int hp) {
		hp_= hp;
	}
	//スキルの攻撃力
	int GetSkillPower() {
		return skillPower_;
	}
	void SetSkillPower(int skillPower) {
		skillPower_ = skillPower;
	}

	//被弾時フラグ
	void SetIsHit(bool isHit) {
		isHit_ = isHit;
	}
	bool GetIsHit() {
		return isHit_;
	}
	//被弾時のクールタイム
	float GetHitCoolTime() {
		return hitCooltime_;
	}

	//左クリックでの攻撃
	bool GetIsLeftAttacking() {
		return isLeftAttacking;
	}
	//現在の弱攻撃のクールタイム
	float GetCurrentWeekAttackCoolTime() {
		return currentWeekAttackCoolTime_;
	}
	//現在のスキル攻撃のクールタイム
	float GetCurrentSkillAttackCoolTime() {
		return currentSkillAttackCoolTime_;
	}

	//プレイヤーのコライダー
	BaseCollider* GetPlayerCollider() {
		return playerCollider;
	}
	//CSVファイル
	bool getFileNames(std::string folderPath,std::vector<std::string>& file_names);

	void LoadPlayerStatusData();
	void UpdatePlayerStatusData();
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
	int power_;
	int skillPower_;

	uint32_t RightBoneNum = 34;
	uint32_t BladeAttackEndPos = 39;

	float hitCooltime_ = 5.0f;

	//弱攻撃のクールタイム(アニメーションに基づいた時間)
	float weekAttackCoolTime_ = 0;
	//現在のクールタイム
	float currentWeekAttackCoolTime_ = 0;


	//スキル攻撃のクールタイム
	float skillAttackCoolTime_ = 600;
	const float MAX_SKILLCOOLTIME = 300;
	//現在のクールタイム
	float currentSkillAttackCoolTime_ = 0;

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
	BaseCollider* playerCloseCollider;
	CollisionManager* collisionManager = nullptr;
	float playerRadius = 0.5f;
	float playerColseRadius = 20.0f;

#pragma endregion


		//プレイヤーの攻撃、体力のコマンド
	std::stringstream playerStatusCommands;


#pragma region

	bool isLeftAttack = false;
	bool isLeftAttacking = false;

	Vector3 DownPos = { 0,20.0f,0 };
	Vector3 DowncolPos = { 0,4.0f,0 };
	Vector3 SunderStartPos = { 0,80.0f,0 };
	Vector3 EnemyPos;

	Vector3 SunderTopPos;
	Vector3 SunderBottomPos;

	std::unique_ptr<Trail3D> trail3D_;

	std::vector<Vector3> SunderRail;


	static const uint32_t SunderAttackColSphereCount = 20;//当たり判定の数
	const float MaxSunderColDetection = 15.0f;

	std::array<WorldTransform,SunderAttackColSphereCount> SunderColWorldTrans;//落雷攻撃のWorldTransform
	std::array<BaseCollider*,SunderAttackColSphereCount> PlayerSunderAttackCollider;//落雷攻撃のコライダー
	//std::unique_ptr<ParticleEditor> particleEditor;

	const float PlayerSunderRadius = 4.0f;

	Vector3 SunderColRatio;

	Vector4 ParticleSunderStartPos;
	Vector4 ParticleSunderEndPos;
	Vector4 ParticleSunderMilEndPos;


#pragma endregion
	std::unique_ptr<Sprite> weekAttackSp_ = nullptr;
	std::unique_ptr<Sprite> weekAttackCoolTimeSp_ = nullptr;
	Vector2 weekAttackCoolTimePos;

	std::unique_ptr<Sprite> skillAttackSp_ = nullptr;//スキル攻撃のスプライト
	std::unique_ptr<Sprite> skillAttackCoolTimeSp_ = nullptr;//スキル攻撃のスプライトのクールタイム
	Vector2 skillAttackCoolTimePos;//スキル攻撃のスプライトの座標




#pragma region
	//オーディオ
	uint32_t NormalSoundNum = 0;
	uint32_t SkillSoundNum = 0;
	float soundVol = 0.08f;

#pragma endregion


};

