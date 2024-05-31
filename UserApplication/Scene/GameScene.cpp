#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
#include"ImGuiManager.h"
#include <CollisionAttribute.h>
#include "Collision.h"
#include"PostEffect.h"
#include"WinApp.h"


GameScene::GameScene() {}
GameScene::~GameScene() {
	collisionManager->AllClearCollider();

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	//当たり判定
	collisionManager = CollisionManager::GetInstance();


	sceneManager_ = SceneManager::GetInstance();

	sprite_ = Sprite::Create(TextureManager::Load("sprite/2.png"));

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
	player_ = std::make_unique<Player>();
	player_->Initialize( COLLISION_ATTR_ALLIES,viewProjection_.get());

	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Initialize(viewProjection_.get(),player_.get(), COLLISION_ATTR_ENEMYS);

	gameCamera = std::make_unique<GameCamera>();
	gameCamera->Initialize(viewProjection_.get(), 0.0f, { 0,0,0 });
	//gameCamera->SetLookDownPos(player_->Getpos());
	gameCamera->SetLookDownDistans();//デフォルトは100
	gameCamera->SetCameraTargetAndPos({ 0,0,0 }, { 0,200,-1 });
	gameCamera->SetFreeCamera(true);

	tile.Initialize();

	randomMap = std::make_unique<RandomMap>();

	randomMap->Initialize();

	std::unique_ptr<Tile> redTile = std::make_unique<Tile>();
	redTile->Initialize();
	redTile->SetSpriteColor({ 1,0,0,1 });

	std::unique_ptr<Tile> blueTile = std::make_unique<Tile>();
	blueTile->Initialize();
	blueTile->SetSpriteColor({ 0,0,1,1 });

	std::unique_ptr<Tile> greenTile = std::make_unique<Tile>();
	greenTile->Initialize();
	greenTile->SetSpriteColor({ 0,1,0,1 });

	std::unique_ptr<Tile> yellowTile = std::make_unique<Tile>();
	yellowTile->Initialize();
	yellowTile->SetSpriteColor({ 0,1,1,1 });

	randomMap->LoadNewTile(std::move(redTile));
	randomMap->LoadNewTile(std::move(blueTile));
	randomMap->LoadNewTile(std::move(greenTile));
	randomMap->LoadNewTile(std::move(yellowTile));

}

void GameScene::Update() {

	if (input_->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}
	enemyManager->Update();
	//gameCamera->SetLookDownPos(player_->Getpos());
	gameCamera->Update();
	player_->Update(input_);

	Vector2 tPos = { 320.0f,180.0f };
	tile.SetSpritePos(tPos);

	tile.Update();

	randomMap->Update();
}

void GameScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(3);


	Model::PreDraw(commandList);


	Model::PostDraw();


	Model::PreDraw(commandList);

	Model::PostDraw();

	player_->FbxDraw(*LightViewProjection.get());

	PostEffect::PostDrawScene();
}

void GameScene::BackgroundDraw()
{
}

void GameScene::CSUpdate()
{


}

bool GameScene::IsBreak()
{
	return isFinishGame;
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();

	//tile.Draw();

	randomMap->Draw();
	sprite_->Draw({ 100,100 }, { 1,1,1,1 }, 1);

#pragma endregion

#pragma region 3Dオブジェクト描画

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	enemyManager->Draw(*LightViewProjection.get());
	player_->Draw(*LightViewProjection.get());
	//3Dオブジェクト描画後処理
	Model::PostDraw();



#pragma endregion

#pragma region 前景スプライト描画



#pragma endregion
}

void GameScene::Finalize()
{
}


bool GameScene::IsSlow()
{
	return false;
}
