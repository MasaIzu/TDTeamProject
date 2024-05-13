#pragma once

#include"Vector3.h"
#include"Matrix4.h"
#include "MyMath.h"


#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <d3d12.h>
#include <wrl.h>
MY_SUPPRESS_WARNINGS_END

// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection {
	Matrix4 view;       // ワールド → ビュー変換行列
	Matrix4 projection; // ビュー → プロジェクション変換行列
	Vector3 cameraPos;  // カメラ座標（ワールド座標）
};

/// <summary>
/// ビュープロジェクションデータ
/// </summary>
struct ViewProjection {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap = nullptr;

#pragma region ビュー行列の設定
	// 視点座標
	Vector3 eye = { 0, 0, -50.0f };
	// 注視点座標
	Vector3 target = { 0, 0, 0 };
	// 上方向ベクトル
	Vector3 up = { 0, 1, 0 };
#pragma endregion

#pragma region 射影行列の設定
	// 垂直方向視野角
	float fovAngleY = ToRadian(60.0f);
	// ビューポートのアスペクト比
	float aspectRatio = (float)16 / 9;
	// 深度限界（手前側）
	float nearZ = 0.1f;
	// 深度限界（奥側）
	float farZ = 4000.0f;
#pragma endregion

	// ビュー行列
	Matrix4 matView;
	// 射影行列
	Matrix4 matProjection;

	Matrix4 matBillboard;

	//カメラの向いてる向き
	Vector3 cameraLook = target.norm();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を更新する
	/// </summary>
	void UpdateMatrix();

	//ラジアンに変更
	float ToRadian(float x) { return x * (MyMath::PI / 180); }
};