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

class Player;

class Enemy
{
public:

	//�R���X�g���N�^�ƃf�X�g���N�^
	Enemy();
	~Enemy();

	//������
	void Initialize(ViewProjection* viewProjection_, Vector3 enemyPos, int actionNmb,Player* player,const unsigned short Attribute);
	//�X�V
	void Update();
	//�`��
	void Draw(const ViewProjection& LightViewProjection_);
	//�ړ�
	void Move();

	//�o���l�̃Z�b�^�[
	void SetExp(int experience) { experience_ = experience; }


	//���S����
	bool IsDead()const { return isDead_; }

	Player* GetPlayer() { return player_; }
	void SetPlayer(Player* player) { player_ = player_; }

	void BulletAttck();

	Vector3 GetPosition();

private:

	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	std::unique_ptr<Model> model_;// 3D���f��
	Player* player_;

	std::list<std::unique_ptr<EnemyBullet>> EnemyBullets_;
	std::unique_ptr<EnemyNormalBulletAttack> enemyNormalBullet;

	Vector3 velocity_;
	float speed = 0.1f;
	int experience_;//�o���l

	//�f�X�t���O
	bool isDead_ = false;

	float enemySpeed = 0.3f;

#pragma region

	bool isHit_ = false;
	unsigned short Attribute_;
	//�����蔻��
	BaseCollider* enemyCollider;
	CollisionManager* collisionManager = nullptr;
	float enemyRadius = 0.5f;

#pragma endregion
};





