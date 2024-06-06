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


SelectScene::SelectScene(int score) {
	score_ = score;
}
SelectScene::~SelectScene() {
	collisionManager->AllClearCollider();
}

void SelectScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	//当たり判定
	collisionManager = CollisionManager::GetInstance();


	sceneManager_ = SceneManager::GetInstance();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/1.png"));
	mouseSprite_ = Sprite::Create(TextureManager::Load("sprite/M_LEFT.png"));

	scoreSprite_ = Sprite::Create(TextureManager::Load("sprite/SCORE.png"));

	rankBSprite_ = Sprite::Create(TextureManager::Load("sprite/b_rank_text.png"));
	rankASprite_ = Sprite::Create(TextureManager::Load("sprite/a_rank_text.png"));
	rankSSprite_ = Sprite::Create(TextureManager::Load("sprite/s_rank_text.png"));

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
	if ( input_->MouseInputTrigger(static_cast< int >( 0 )) )
	{
		sceneManager_->ChangeScene("TITLE",0);
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

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region /*背景スプライト描画*/

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
	mouseSprite_->Draw({ 625, 600 },{ 1,1,1,1 },1);

	if ( score_ <= 10 )
	{
		rankBSprite_->Draw({ 1000,150 },{ 1,1,1,1 },1);
	}
	else if ( score_ > 10 && score_ <= 20 )
	{
		rankASprite_->Draw({ 1000,150 },{ 1,1,1,1 },1);
	}
	else if ( score_ > 20 )
	{
		rankSSprite_->Draw({ 1000,150 },{ 1,1,1,1 },1);
	}
	scoreSprite_->Draw({500,100},{ 1,1,1,1 },1);

#pragma endregion

#pragma region /*3Dオブジェクト描画*/

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);


	//3Dオブジェクト描画後処理
	Model::PostDraw();



#pragma endregion

#pragma region /*前景スプライト描画*/



#pragma endregion
}

void SelectScene::Finalize()
{
}


bool SelectScene::IsSlow()
{
	return false;
}
