#pragma once
#include <memory>
#include <list>
#include <cassert>
#include "EnemyBullet.h"
#include "Model.h"

class Enemy;
class Player;
class EnemyNormalBulletAttack
{
public:
	EnemyNormalBulletAttack();
	~EnemyNormalBulletAttack();


/// <summary>
/// 初期化
/// </summary>
	void Initialize(Model* bulletModel);


	/// <summary>
	/// 更新
	/// </summary>
	void Update(Enemy* enemy,Player* player);



	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& LightViewProjection_);

	void Attck(ViewProjection* viewProjection);

private:
	//敵の弾モデル
	std::unique_ptr<Model> enemyBulletModel_ = nullptr;

	Enemy* enemy_ = nullptr;

	Player* player_ = nullptr;
	float playerWidth = 1.5f;
	float enemyWidth_ = 1.5f;

	float move = 0.1f;

	std::list<std::unique_ptr<EnemyBullet>> EnemyBullets_;



	Vector3 velocity_;
	float verocitySpeed = 1.5f;
};
