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

	void CheckHitCollision();

	void HpUpdate();
	
	int GetHp() { return hp_; }

	//�o���l�̑����֐�
	void AddExperience(int amount);

	//���x���A�b�v�֐�
	void LevelUp();

	//���̃��x���A�b�v�ɕK�v�Ȍo���l���v�Z(���䐔��Ōv�Z)
	int CalculateNextLevelExperience() const;

	//�v���[���[�̍U��
	void PlayerBladeAttack();

	//�A�^�b�N�A�b�v�f�[�g
	void AttackUpdate();
	//���̓����蔻�葮���X�V
	void BladeAttributeSet(const unsigned short Attribute_);

	int GetLevel() const { return level; }
	int GetExperience() const { return experience; }
	int GetExperienceToNextLevel() const { return experienceToNextLevel; }

private:
	Input* input_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	std::unique_ptr<Model> model_;// 3D���f��
	std::unique_ptr<FBXObject3d> playerFbx_;
	//�A�j���[�V�����N���X
	std::unique_ptr<Animation> animation;
	std::unique_ptr<Animation> animation2;
	Vector3 velocity_;
	float speed = 0.7f;


	int hp_ = 100;
	bool isAlive_ = true;
	bool isHit_ = false;

	uint32_t RightBoneNum = 34;
	uint32_t BladeAttackEndPos = 39;

	float hitCooltime_ = 5.0f;

#pragma region
	//�A�j���[�V�����^�C��
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
	const float MaxBladeColDetection = 100.0f;

	std::array<WorldTransform, AttackColSphereCount> BladeColWorldTrans;
	std::array<BaseCollider*, AttackColSphereCount> PlayerBladeAttackCollider;

	float PlayerBladeRadius = 1.0f;

	Vector3 BladeColRatio;

	Vector4 ParticleStartPos;
	Vector4 ParticleEndPos;
	Vector4 ParticleMilEndPos;

#pragma endregion

#pragma region
	int level = 1;//���x��
	int experience = 0;//�o���l
	int experienceToNextLevel;//���̃��x���ɕK�v�Ȍo���l
	const int baseExperience = 50;
	const double ratio = 1.2;
#pragma endregion

#pragma region
	unsigned short Attribute_;
	//�����蔻��
	BaseCollider* playerCollider;
	CollisionManager* collisionManager = nullptr;
	float playerRadius = 0.5f;
#pragma endregion
};

