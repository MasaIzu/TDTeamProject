#pragma once

#pragma once

#include "Sprite3D.h"

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"
#include "CollisionManager.h"

#include<memory>
#include<vector>

class Player;

class Enemy
{
public:

	//コンストラクタとデストラクタ
	Enemy();
	~Enemy();

	//初期化
	void Initialize(ViewProjection* viewProjection_,Player * player);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& LightViewProjection_);
	//移動
	void Move();

	//経験値のセッター
	void SetExp(int experience) { experience_ = experience; }

	//プレイヤーのsetter
	void SetPlayer(Player* player) { player_ = player; }


private:

	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	std::unique_ptr<Model> model_;// 3Dモデル
	CollisionManager* collisionManager = nullptr;//当たり判定

	Player* player_ = nullptr;

	Vector3 velocity_;
	float speed = 0.1f;
	int experience_;//経験値
};





