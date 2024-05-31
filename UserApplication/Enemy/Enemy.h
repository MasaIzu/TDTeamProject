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

	//�R���X�g���N�^�ƃf�X�g���N�^
	Enemy();
	~Enemy();

	//������
	void Initialize(ViewProjection* viewProjection_, Vector3 enemyPos, int actionNmb,Player* player);
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

private:

	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	std::unique_ptr<Model> model_;// 3D���f��
	CollisionManager* collisionManager = nullptr;//�����蔻��
	Player* player_;


	Vector3 velocity_;
	float speed = 0.1f;
	int experience_;//�o���l

	//�f�X�t���O
	bool isDead_ = false;
};





