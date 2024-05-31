#pragma once

#include "DirectXCore.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "BaseScene.h"
#include "SceneManager.h"

#include<memory>
#include<vector>
#include <CollisionManager.h>
#include "LoadLevelEditor.h"

#include "TouchableObject.h"

class TitleScene : public BaseScene
{
public: // �����o�֐�
	TitleScene();
	~TitleScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;
	// �I������
	void Finalize() override;

	//�|�X�g�G�t�F�N�g���|������
	void PostEffectDraw() override;
	//�w�i�`��
	void BackgroundDraw() override;
	//CS�A�b�v�f�[�g
	void CSUpdate() override;
	//�u���C�N���邩
	bool IsBreak() override;


	//�X���E
	bool IsSlow() override;

private: // �����o�ϐ�
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<ViewProjection> viewProjection_;//�r���[�v���W�F�N�V����
	std::unique_ptr<ViewProjection> LightViewProjection;//���C�g�r���[�v���W�F�N�V����
	std::unique_ptr<Model> model_;// 3D���f��
	WorldTransform worldTransform_;//���[���h�ϊ��f�[�^
	CollisionManager* collisionManager = nullptr;//�����蔻��
	SceneManager* sceneManager_ = nullptr;//�V�[���}�l�[�W���[
	std::unique_ptr<LoadLevelEditor> levelData;
	std::unique_ptr<TouchableObject> touchableObject;

	bool isFinishGame = false;

	uint32_t textureHandle_ = 0;//�e�N�X�`���n���h��

	uint32_t titleTexture_;
	std::unique_ptr<Sprite> titleSprite_ = nullptr;

};

