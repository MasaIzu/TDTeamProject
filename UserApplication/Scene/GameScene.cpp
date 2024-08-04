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
#include <AudioManager.h>


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

	//timeSprite_ = Sprite::Create(TextureManager::Load("sprite/Green30.png"));

	mouseLSprite_ = Sprite::Create(TextureManager::Load("sprite/M_LEFT.png"));
	mouseLSprite_->SetSize({ 100.0f,100.0f });

	mouseRSprite_ = Sprite::Create(TextureManager::Load("sprite/M_RIGHT.png"));
	mouseRSprite_->SetSize({ 100.0f,100.0f });

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,0,-50 };
	viewProjection_->UpdateMatrix();

	LightViewProjection = std::make_unique<ViewProjection>();
	LightViewProjection->Initialize();
	LightViewProjection->eye = { 0,100,50 };
	LightViewProjection->UpdateMatrix();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,0 };
	worldTransform_.scale_ = { 1000.0f,1000.0f,1000.0f };
	worldTransform_.TransferMatrix();

	worldTransformJimen_.Initialize();
	worldTransformJimen_.translation_ = { 0,0,0 };
	worldTransformJimen_.TransferMatrix();

	skydome.reset(Model::CreateFromOBJ("skydome",true));

	BGMSoundNum = AudioManager::GetInstance()->LoadAudio("Resources/Sound/game01.mp3",soundVol,false);

	collisionManager = CollisionManager::GetInstance();
	player_ = std::make_unique<Player>();
	player_->Initialize(COLLISION_ATTR_ALLIES,viewProjection_.get());

	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Initialize(viewProjection_.get(),player_.get(),COLLISION_ATTR_ENEMYS);

	gameCamera = std::make_unique<GameCamera>();
	gameCamera->Initialize(viewProjection_.get(),0.0f,{ 0,0,0 });
	gamecameraDistans_ = 100.0f;
	gameCameraPosition_ = player_->GetPosition();
	gameCamera->SetLookDownPos(gameCameraPosition_);
	gameCamera->SetLookDownDistans(gamecameraDistans_);

	startBanner = std::make_unique<Banner>();
	startBanner->Initialize();

	ui_ = std::make_unique<UI>();
	ui_->Initialize();
	ui_->SetPlayerHP(player_->GetHp());

//	tile.Initialize();

	randomMap = std::make_unique<RandomMap>();

	randomMap->Initialize();

	//タイルクラスにカメラ行列とかをセット
	Tile::SetLightView(*LightViewProjection.get());
	Tile::SetViewProjection(*viewProjection_.get());

	std::unique_ptr<Tile> redTile = std::make_unique<Tile>();
	redTile->Initialize("sphereRed");
	redTile->SetSpriteColor({ 1,0,0,1 });


	std::unique_ptr<Tile> blueTile = std::make_unique<Tile>();
	blueTile->Initialize("sphereBlue");
	blueTile->SetSpriteColor({ 0,0,1,1 });

	std::unique_ptr<Tile> greenTile = std::make_unique<Tile>();
	greenTile->Initialize("sphereGreen");
	greenTile->SetSpriteColor({ 0,1,0,1 });

	std::unique_ptr<Tile> yellowTile = std::make_unique<Tile>();
	yellowTile->Initialize("sphereYellow");
	yellowTile->SetSpriteColor({ 0,1,1,1 });

	randomMap->LoadNewTile(std::move(redTile));
	randomMap->LoadNewTile(std::move(blueTile));
	randomMap->LoadNewTile(std::move(greenTile));
	randomMap->LoadNewTile(std::move(yellowTile));

	timeGauge = 3600;
	ui_->SetTimeRest(timeGauge);
	ui_->SetTimeGauge(timeGauge);
	ui_->SetPlayerStartHP(player_->GetHp());

	Jimen.reset(Model::CreateFromOBJ("jimen",true));
	AudioManager::GetInstance()->PlayWave(BGMSoundNum,true);
}

void GameScene::Update() {
	ui_->SetPlayerHP(player_->GetHp());
	ui_->SetTimeGauge(timeGauge);
	ui_->SetNowPhase(nowPhase);
	ui_->SetSkillAttackCoolTime(player_->GetCurrentSkillAttackCoolTime());
	ui_->SetWeekAttackCoolTime(player_->GetCurrentWeekAttackCoolTime());
	ui_->SetIsLeftAttacking(player_->GetIsLeftAttacking());

	if ( startBanner->GetAnimEnd() == true )
	{
		if ( player_->GetPlayerCollider()->GetHitSphere() )//敵とぶつかった際に
		{
			if ( player_->GetHitCoolTime() == 30.0f )
			{
				timeGauge -= 60;//制限時間を1秒分減らす
			}
		}
		timeGauge--;

		enemyManager->SetPhase(nowPhase);

		score_ = player_->GetScore();
		Phase(timeGauge);
		if ( timeGauge <= 0 )
		{
			AudioManager::GetInstance()->StopWave(BGMSoundNum);
			sceneManager_->ChangeScene("SELECT",score_);
			timeGauge = 0;
		}

		gameCamera->SetLookDownPos(player_->GetPosition());
		player_->Update(input_);
		enemyManager->Update();
	}
	gameCamera->SetLookDownDistans(gamecameraDistans_);
	gameCamera->Update();
	worldTransform_.TransferMatrix();


	//timeSprite_->SetSize({ (float)timeGauge,50.0f });
//	Vector2 tPos = { 320.0f,180.0f };
	//tile.SetSpritePos(tPos);

	//tile.Update();

	randomMap->Update();

	startBanner->Update();

	ui_->Update(startBanner->GetAnimEnd());

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();
}

void GameScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	PostEffect::SetShadeNumber(3);


	Model::PreDraw(commandList);


	Model::PostDraw();


	Model::PreDraw(commandList);

	Model::PostDraw();


	player_->ParticleDraw();
	enemyManager->ParticleDraw();

	player_->TarilDraw();

}

void GameScene::BackgroundDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Model::PreDraw(commandList);

	//Jimen->Draw(worldTransformJimen_,*viewProjection_,*LightViewProjection.get());

	Model::PostDraw();
}

void GameScene::CSUpdate()
{
	player_->CSUpdate(DirectXCore::GetInstance()->GetCommandList());
	enemyManager->CSUpdate(DirectXCore::GetInstance()->GetCommandList());

}

bool GameScene::IsBreak()
{
	return isFinishGame;
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画

	//tile.Draw();

	if ( startBanner->GetAnimEnd() == true )
	{
		mouseLSprite_->Draw({ 100,600 },{ 1,1,1,1 },1);
		mouseRSprite_->Draw({ 1000,600 },{ 1,1,1,1 },1);
		player_->SpriteDraw();
	}
//	timeSprite_->Draw({ 600,100 },{ 1,1,1,1 },1);

#pragma endregion

#pragma region 3Dオブジェクト描画

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	skydome->Draw(worldTransform_,*viewProjection_,*LightViewProjection.get());
	Jimen->Draw(worldTransformJimen_,*viewProjection_,*LightViewProjection.get());
	enemyManager->Draw(*LightViewProjection.get());
	player_->Draw(*LightViewProjection.get());



	randomMap->Draw();


	//3Dオブジェクト描画後処理
	Model::PostDraw();

	player_->FbxDraw(*LightViewProjection.get());


#pragma endregion

#pragma region 前景スプライト描画

	startBanner->Draw();
	ui_->Draw();
	//sprite_->Draw({ 100,100 },{ 1,1,1,1 },1);

#pragma endregion
}

void GameScene::Finalize()
{
}


bool GameScene::IsSlow()
{
	return false;
}

void GameScene::Phase(int32_t gametime) {
	int32_t time;
	if ( gametime != 0 )
	{
		time = 1200 / gametime;
	}

	switch ( time )
	{
	case 1:
		nowPhase = 1;
		break;
	case 2:
		nowPhase = 2;
		break;
	case 3:
		nowPhase = 3;
	default:
		if ( time > 3 )
		{
			nowPhase = 3;
		}
		break;
	}

}