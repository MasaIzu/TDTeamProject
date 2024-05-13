#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Uint32Vector2.h"
#include <Input.h>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <BaseCollider.h>

/// <summary>
/// ゲームプレイカメラ
/// </summary>
class DebugCamera
{

public://基本関数

	DebugCamera();
	~DebugCamera();

	//初期化
	void Initialize(ViewProjection* viewProjection_);
	//更新
	void Update();

	void CameraUpdate();

private://クラス関連

	Input* input_ = nullptr;
	ViewProjection* viewProjection = nullptr;

private://プレイヤークラス変数

	const float LerpStrength = 0.09f;

	bool cameraMode = false;
	bool isPushMouse = false;

	uint32_t cameraType = 0;
	uint32_t winWidth = 0;
	uint32_t winHeight = 0;
	uint32_t cameraTime = 0;
	uint32_t MaxCameraTime = 0;
	uint32_t cameraMode_ = 0;//カメラモード
	uint32_t HowMachMovePointer = 0;
	uint32_t shakeTime = 0;
	uint32_t cameraModeChangeCountTimer = 30;

	float scaleX_ = 1.0f;// スケーリング
	float scaleY_ = 1.0f;
	float angleAroundPlayer = 0.0f; // プレイヤーの周りを回転する角度
	float cameraDistance_ = 35.0f;
	float angle = 0.0f;
	float cameraSpeed_ = 3;// カメラの速度
	float cameraDelay = 0.1f;// カメラが追跡する際の遅延量
	float playerCameraDelay = 0.1f;
	float playerCameraSpeed_ = 3;
	float kand = 400.0f;
	float Fov = 45.0f;
	float TargetCameraDelay = 0.05f;
	float TargetCameraSpeed_ = 1.0;
	float CameraDistanceMinus = 0.0f;
	float CameraMouseMoved = 0.0f;
	float OldMouseMoved = 0.0f;
	float CameraCollisionRadius = 0.1f;
	float CameraRayCollisionRadius = 0.2f;
	float PlayerToCameraVecDistance = 0.0f;

	Uint32Vector2 windowWH;

	Vector2 mousePos;
	Vector2 oldMousePos;
	Vector2 Mous_UP_DOWN;
	Vector2 MouseMove;
	Vector2 mouseMoved{ 0, MyMath::PI };
	Vector2 ConVec;
	Vector2 OldPos;

	Vector3 CameraPos;
	Vector3 rot;
	Vector3 eye;
	Vector3 vUp;
	Vector3 target;
	Vector3 PlayerToCameraVec;
	//セットされたVec
	Vector3 SetTargetVec;
	Vector3 SetEyeVec;
	//遅延カメラ
	Vector3 LerpPlayerPosition;

	Matrix4 CameraRot;
	Matrix4 matRot;// 回転行列

};