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

#include"Tile.h"
#include"RandomMap.h"

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

private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<ViewProjection> viewProjection_;//ビュープロジェクション
	std::unique_ptr<ViewProjection> LightViewProjection;//ライトビュープロジェクション
	std::unique_ptr<Model> model_;// 3Dモデル
	WorldTransform worldTransform_;//ワールド変換データ
	CollisionManager* collisionManager = nullptr;//当たり判定
	SceneManager* sceneManager_ = nullptr;//シーンマネージャー
	std::unique_ptr<LoadLevelEditor> levelData;
	std::unique_ptr<TouchableObject> touchableObject;

	bool isFinishGame = false;

	uint32_t textureHandle_ = 0;//テクスチャハンドル

	Tile tile;

	std::unique_ptr<RandomMap> randomMap = nullptr;

	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
};