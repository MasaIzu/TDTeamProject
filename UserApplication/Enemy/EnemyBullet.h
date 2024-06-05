#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class EnemyBullet
{
public:
	EnemyBullet();
	~EnemyBullet();
	///<summary>
	///������
	///</summary>

	void Initialize(const Vector3& position, const Vector3& velocity, Model* bulletModel, ViewProjection* viewProjection_);



	///<summary>
	///�X�V
	///</summary>

	void Update();

	///<summary>
	///�`��
	///</summary>

	void Draw(const ViewProjection& LightViewProjection_);

	///<summary>
	////�e����
	///</summary>
	bool IsDead()const { return isDead_; }

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	///<summary>
	////�G�̍��W
	///</summary>
	Vector3 GetWorldPosition();

	///<summary>
	////�e�̃T�C�Y��ς���
	///</summary>
	void SetSize(Vector3 Size) { worldTransform_.scale_ = Size; }

	///<summary>
	////�e�̐i�s�����ɒe�Ɍ�����ς���
	///</summary>
	void VecRot();

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	std::unique_ptr<Model> model_;// 3D���f��

	//���x
	Vector3 velocity_;

	//����<fim>
	static const int32_t kLifeTime = 60 * 30;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	int32_t deathTimerEnd_ = 0;
	//�f�X�t���O
	bool isDead_ = false;

private:

};