#pragma once
#include "WinApp.h"
#include"DirectXCore.h"

class SceneManager;


//シーン基底クラス
class BaseScene {

public:
	//virtualメンバ関数
	//初期化
	virtual void Initialize() = 0;

	//更新
	virtual void Update() = 0;

	//ポストエフェクト描画
	virtual void PostEffectDraw() = 0;

	//ポストエフェクト描画
	virtual void BackgroundDraw() = 0;

	//描画
	virtual void Draw() = 0;

	//描画
	virtual void Finalize() = 0;

	//CSアップデート
	virtual void CSUpdate() = 0;

	//ブレイクするかどうか
	virtual bool IsBreak() = 0;

	//ブレイクするかどうか
	virtual bool IsSlow() = 0;



public://メンバ関数

	virtual ~BaseScene() = default;

	//シーンセット
	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; };


private:
	//シーンマネージャー
	SceneManager* sceneManager_ = nullptr;

};