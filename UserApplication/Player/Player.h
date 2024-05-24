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

	//�R���X�g���N�^�ƃf�X�g���N�^
	Player();
	~Player();

	//������
	void Initialize(ViewProjection* viewProjection_);
	//�X�V
	void Update(Input* input);
	//�`��
	void Draw(const ViewProjection& LightViewProjection_);

	void Move(Input* input);

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
	CollisionManager* collisionManager = nullptr;//�����蔻��
	std::unique_ptr<FBXObject3d> playerFbx_;

	Vector3 velocity_;
	float speed = 0.1f;

	int level = 1;//���x��
	int experience = 0;//�o���l
	int experienceToNextLevel;//���̃��x���ɕK�v�Ȍo���l
	const int baseExperience = 50;
	const double ratio = 1.2;
};

