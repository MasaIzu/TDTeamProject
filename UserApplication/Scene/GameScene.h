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

#include "UserApplication/Player/Player.h";
#include "UserApplication/Enemy/Enemy.h";
#include "UserApplication/GameCam/GameCamera.h";

#include"Tile.h"
#include"RandomMap.h"

#include "CSVLoader.h"
#include "EnemyManager.h"

#include "Banner.h"
#include "UI.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {

public: // メンバ関数

	GameScene();
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	// 終了処理
	void Finalize() override;

	//ポストエフェクトを掛けるやつ
	void PostEffectDraw() override;
	//背景描画
	void BackgroundDraw() override;
	//CSアップデート
	void CSUpdate() override;
	//ブレイクするか
	bool IsBreak() override;


	//スロウ
	bool IsSlow() override;

private:
	void Phase(int32_t gametime);

	int32_t GetPhase() {
		return nowPhase;
	}

private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<ViewProjection> viewProjection_;//ビュープロジェクション
	std::unique_ptr<ViewProjection> LightViewProjection;//ライトビュープロジェクション
	std::unique_ptr<Model> model_;// 3Dモデル
	WorldTransform worldTransform_;//ワールド変換データ
	WorldTransform worldTransformJimen_;//ワールド変換データ
	CollisionManager* collisionManager = nullptr;//当たり判定
	SceneManager* sceneManager_ = nullptr;//シーンマネージャー
	std::unique_ptr<LoadLevelEditor> levelData;
	std::unique_ptr<TouchableObject> touchableObject;

	std::unique_ptr<EnemyManager> enemyManager;//エネミーマネージャー

	bool isFinishGame = false;

	uint32_t textureHandle_ = 0;//テクスチャハンドル

	Tile tile;

	std::unique_ptr<RandomMap> randomMap = nullptr;

	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
	Enemy* enemys_[10] = {};

	CSVLoader* csv[10]={};

	std::unique_ptr<GameCamera> gameCamera;
	Vector3 gameCameraPosition_;
	float gamecameraDistans_;

	std::unique_ptr<Model> skydome;
	std::unique_ptr<Model> Jimen;
	std::unique_ptr<Sprite> mouseSprite_ = nullptr;
	std::unique_ptr<Banner> startBanner;

	std::unique_ptr<UI> ui_;

	std::unique_ptr<Sprite> timeSprite_ = nullptr;
	int score_;
	int timeGauge = 1200;

	int32_t nowPhase;
};