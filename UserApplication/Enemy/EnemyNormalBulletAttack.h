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
	//EnemyBulletAttack();
	//~EnemyBulletAttack();


/// <summary>
/// ������
/// </summary>
	void Initialize(Model* bulletModel);


	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Enemy* enemy);



	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& LightViewProjection_);

	void Attck(ViewProjection* viewProjection);

private:
	//�G�̒e���f��
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
