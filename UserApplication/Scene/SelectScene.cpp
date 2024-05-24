#include "SelectScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
#include"ImGuiManager.h"
#include <CollisionAttribute.h>
#include "Collision.h"
#include"PostEffect.h"
#include"WinApp.h"


SelectScene::SelectScene() {}
SelectScene::~SelectScene() {
	collisionManager->AllClearCollider();
}

void SelectScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	//�����蔻��
	collisionManager = CollisionManager::GetInstance();


	sceneManager_ = SceneManager::GetInstance();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/1.png"));

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-50 };
	viewProjection_->UpdateMatrix();

	LightViewProjection = std::make_unique<ViewProjection>();
	LightViewProjection->Initialize();
	LightViewProjection->eye = { 0,100,0 };
	LightViewProjection->UpdateMatrix();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 50,0,0 };
	worldTransform_.scale_ = { 10.0f,3.0f,4.0f };
	worldTransform_.TransferMatrix();


	collisionManager = CollisionManager::GetInstance();

}

void SelectScene::Update() {

	int a = 0;
	if (input_->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}
}

void SelectScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(3);


	Model::PreDraw(commandList);


	Model::PostDraw();


	Model::PreDraw(commandList);

	Model::PostDraw();



	PostEffect::PostDrawScene();
}

void SelectScene::BackgroundDraw()
{
}

void SelectScene::CSUpdate()
{


}

bool SelectScene::IsBreak()
{
	return isFinishGame;
}

void SelectScene::Draw() {

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region /*�w�i�X�v���C�g�`��*/

	// �[�x�o�b�t�@�N���A
	dxCommon_->ClearDepthBuffer();
	sprite_->Draw({ 100,100 }, { 1,1,1,1 }, 1);

#pragma endregion

#pragma region /*3D�I�u�W�F�N�g�`��*/

	//// 3D�I�u�W�F�N�g�`��O����
	Model::PreDraw(commandList);


	//3D�I�u�W�F�N�g�`��㏈��
	Model::PostDraw();



#pragma endregion

#pragma region /*�O�i�X�v���C�g�`��*/



#pragma endregion
}

void SelectScene::Finalize()
{
}


bool SelectScene::IsSlow()
{
	return false;
}
