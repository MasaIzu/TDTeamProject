#pragma once

#include "Sprite3D.h"

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"
#include "CollisionManager.h"
#include "FBXObject3d.h"

#include<memory>
#include<vector>

class Player
{
public:

	//コンストラクタとデストラクタ
	Player();
	~Player();

	//初期化
	void Initialize(ViewProjection* viewProjection_);
	//更新
	void Update(Input* input);
	//描画
	void Draw(const ViewProjection& LightViewProjection_);

	void Move(Input* input);

	//描画
	void FbxDraw(const ViewProjection& lightViewProjection_);
	//描画
	void FbxShadowDraw(const ViewProjection& lightViewProjection_);

	//経験値の増加関数
	void AddExperience(int amount);

	//レベルアップ関数
	void LevelUp();

	//次のレベルアップに必要な経験値を計算(等比数列で計算)
	int CalculateNextLevelExperience() const;

	int GetLevel() const { return level; }
	int GetExperience() const { return experience; }
	int GetExperienceToNextLevel() const { return experienceToNextLevel; }

private:

	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	std::unique_ptr<Model> model_;// 3Dモデル
	CollisionManager* collisionManager = nullptr;//当たり判定
	std::unique_ptr<FBXObject3d> playerFbx_;

	Vector3 velocity_;
	float speed = 0.1f;

	int level = 1;//レベル
	int experience = 0;//経験値
	int experienceToNextLevel;//次のレベルに必要な経験値
	const int baseExperience = 50;
	const double ratio = 1.2;
};

