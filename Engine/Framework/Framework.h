#pragma once
#include "DirectXCore.h"
#include "GameScene.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "Input.h"
#include "FPS.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
//#include "FbxModel.h"
//#include <FbxLoader.h>

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <memory>
#include <ShadowMap.h>
#include <AudioManager.h>
MY_SUPPRESS_WARNINGS_END

/// <summary>
/// フレームワーク
/// </summary>
class Framework {
public:
	//virtualメンバ関数
	//初期化
	virtual void Initialize();

	//更新
	virtual void Update();

	//ポストエフェクト
	virtual void PostEffectDraw()=0;

	//描画
	virtual void Draw() = 0;

	//終了処理
	virtual void Finalize();

	//終了チェック
	virtual bool isPlayMyGame();

	//スロウ
	virtual bool isGameSlow();

	//CSアップデート
	virtual void CSUpdate() = 0;

	//ブレイクするかどうか
	virtual bool IsBreak() = 0;

	//ブレイクするかどうか
	virtual bool IsSlow() = 0;

public://デストラクタ
	virtual ~Framework() = default;

public:
	//メンバ関数
	//実行
	void Run();

	WinApp* GetWinApp() { return winApp_; }
	DirectXCore* GetDirectXCore() { return directXCore_; }

public:
	static uint32_t Time;

protected:

	//シングルトン
	WinApp* winApp_ = nullptr;
	DirectXCore* directXCore_ = nullptr;
	Input* input_ = nullptr;
	SceneManager* sceneManager_ = nullptr;
	TextureManager* TextureManager_ = nullptr;
	/*FbxLoader* fbxLoader_ = nullptr;*/

	//シングルトン以外
	std::unique_ptr<ImGuiManager> imGui;
	std::unique_ptr <FPS> fps;
	std::unique_ptr <AbstractSceneFactory> sceneFactory_;
	//シャドウマップ(影生成ライト視点用)
	std::unique_ptr<ShadowMap> shadowMap;
	AudioManager* audioManager = nullptr;
	bool isEndRequst = false;

	bool isSlow = false;
	bool isFirstUpdate = true;

	uint32_t NotUpdateTime = 0;
	uint32_t MaxNotUpdateTime = 3;

};