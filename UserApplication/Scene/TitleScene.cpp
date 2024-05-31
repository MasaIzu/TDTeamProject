#include "TitleScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
#include"ImGuiManager.h"
#include <CollisionAttribute.h>
#include "Collision.h"
#include"PostEffect.h"
#include"WinApp.h"


TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	collisionManager->AllClearCollider();
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	//当たり判定
	collisionManager = CollisionManager::GetInstance();


	sceneManager_ = SceneManager::GetInstance();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/0.png"));
	titleSprite_ = Sprite::Create(TextureManager::Load("sprite/title.png")); ;

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

void TitleScene::Update() {

	int a = 0;
	if (input_->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("SELECT");
	}
}

void TitleScene::PostEffectDraw()
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

void TitleScene::BackgroundDraw()
{
}

void TitleScene::CSUpdate()
{


}

bool TitleScene::IsBreak()
{
	return isFinishGame;
}

void TitleScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region /*背景スプライト描画*/

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();

	sprite_->Draw({ 100,100 }, { 1,1,1,1 }, 1);
	titleSprite_->Draw({625, 325 }, { 1,1,1,1 }, 1);
#pragma endregion

#pragma region/* 3Dオブジェクト描画*/

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);


	//3Dオブジェクト描画後処理
	Model::PostDraw();



#pragma endregion

#pragma region /*前景スプライト描画*/



#pragma endregion
}

void TitleScene::Finalize()
{
}


bool TitleScene::IsSlow()
{
	return false;
}
