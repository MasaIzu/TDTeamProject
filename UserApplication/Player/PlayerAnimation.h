#pragma once

#include "WorldTransform.h"
#include <FbxModel.h>
#include <FBXObject3d.h>

class PlayerAnimation
{
public://��{�֐�
	PlayerAnimation();
	~PlayerAnimation();

	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void FbxDraw(const WorldTransform& worldTransform, const ViewProjection& viewProjection_, const ViewProjection& lightViewProjection_);
	void FbxShadowDraw(const WorldTransform& worldTransform, const ViewProjection& lightViewProjection_);
private://�����o�֐�


public://Setter

	void SetAnimation(const uint32_t& animNumber, const uint32_t& startCount, const uint32_t& maxCount, const bool& loop);
	void SetKeepAnimation(const uint32_t& animNumber, const uint32_t& Count, const uint32_t& maxCount);
public://Getter
	bool GetNowAnimFinish();
	bool GetAnimAlmostOver(const float& count);
	Matrix4 GetBonePos(const uint32_t& boneNum);

private://�N���X�֘A
	std::unique_ptr<FBXModel> fbxModel_;
	std::unique_ptr<FBXObject3d> fbxObj3d_;

private://�C�[�i���N���X



private://�ʃN���X����l�����炤


private://�v���C���[�N���X�ϐ�

	bool isLoop = false;
	bool isStart = false;
	bool isMotionFinish = true;

	uint32_t AnmNumNumber_ = 7;
	uint32_t nowAnmFCount_ = 0;
	uint32_t maxFcount = 60;
};

