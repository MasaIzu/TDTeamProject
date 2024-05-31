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

class Enemy;

class Player
{
public:

	//�R���X�g���N�^�ƃf�X�g���N�^
	Player();
	~Player();

	//������
	void Initialize(const unsigned short Attribute,ViewProjection* viewProjection_);
	//�X�V
	void Update(Input* input);
	//�`��
	void Draw(const ViewProjection& LightViewProjection_);

	void Move(Input* input);

	Vector3 GetPosition() { return worldTransform_.translation_; }

	//�`��
	void FbxDraw(const ViewProjection& lightViewProjection_);
	//�`��
	void FbxShadowDraw(const ViewProjection& lightViewProjection_);

	//�o���l�̑����֐�
	void AddExperience(int amount);

	//���x���A�b�v�֐�
	void LevelUp();

	//���̃��x���A�b�v�ɕK�v�Ȍo���l���v�Z(���䐔��Ōv�Z)
	int CalculateNextLevelExperience() const;

	int GetLevel() const { return level; }
	int GetExperience() const { return experience; }
	int GetExperienceToNextLevel() const { return experienceToNextLevel; }

private:

	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	std::unique_ptr<Model> model_;// 3D���f��
	std::unique_ptr<FBXObject3d> playerFbx_;
	//�A�j���[�V�����N���X
	std::unique_ptr<Animation> animation;
	Vector3 velocity_;
	float speed = 0.1f;


#pragma region
	int level = 1;//���x��
	int experience = 0;//�o���l
	int experienceToNextLevel;//���̃��x���ɕK�v�Ȍo���l
	const int baseExperience = 50;
	const double ratio = 1.2;
#pragma endregion

#pragma region
	bool isHit_ = false;
	unsigned short Attribute_;
	//�����蔻��
	BaseCollider* playerCollider;
	CollisionManager* collisionManager = nullptr;
	float playerRadius = 0.5f;
#pragma endregion
};

