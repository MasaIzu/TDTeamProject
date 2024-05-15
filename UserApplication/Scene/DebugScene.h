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
#include "ParticleEditor.h"
#include <DebugCamera.h>
#include "AddShield.h"

/// <summary>
/// デバッグシーン
/// </summary>
class DebugScene : public BaseScene {

public: // メンバ関数

	DebugScene();
	~DebugScene();

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

	std::unique_ptr<ViewProjection> viewProjection_;//ビュープロジェクション
	std::unique_ptr<ViewProjection> LightViewProjection;//ライトビュープロジェクション
	std::unique_ptr<DebugCamera> debugCamera;
	std::unique_ptr<ParticleEditor> particleEditor;
	std::unique_ptr<AddShield> AddShield_;

	int shadeNumber = 3;
	int range = 0;//ぼかし強度
	int samples = 0;
	bool isFinishGame = false;
	bool isSlowGame = false;

	uint32_t MaxFream = 60;

	float intensity = 0.1f;
	float angle = 45.0f;
	float angle2 = 135.0f;
	float cameraPos = -100.0f;

	Vector2 center = { 0.5f,0.5f };

	Vector3 CameraPos;

};