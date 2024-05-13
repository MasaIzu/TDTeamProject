#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <memory>
MY_SUPPRESS_WARNINGS_END

/// <summary>
/// スプライト
/// </summary>
class Sprite {

public: // サブクラス
	/// <summary>
	/// 頂点データ構造体
	/// </summary>
	struct VertexPosUv {
		Vector3 pos; // xyz座標
		Vector2 uv;  // uv座標
	};

	/// <summary>
	/// 定数バッファ用データ構造体
	/// </summary>
	struct ConstBufferData {
		Vector4 color; // 色 (RGBA)
		Matrix4 mat;   // ３Ｄ変換行列
	};

public: // 静的メンバ関数

	// 静的初期化
	static void StaticInitialize(ID3D12Device* device,const std::wstring& directoryPath = L"Resources/");

	// 静的終了処理
	static void StaticFinalize();

	// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList,const int& blendMode = 1);

	/// 描画後処理
	static void PostDraw();


	// スプライト生成
	static std::unique_ptr<Sprite> Create(const uint32_t& textureHandle);

private: // 静的メンバ変数
	// 頂点数
	static const int kVertNum = 4;
	// デバイス
	static ID3D12Device* Device_;
	// デスクリプタサイズ
	static UINT DescriptorHandleSize_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* CommandList_;
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature_;
	// パイプラインステートオブジェクト
	static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, 6>PipelineStates_;
	// 射影行列
	static Matrix4 Matrix4Projection_;

public: // メンバ関数

	// コンストラクタ
	Sprite();
	// コンストラクタ
	Sprite(const uint32_t& textureHandle,const Vector2& size);


	// 初期化
	bool Initialize();

	// テクスチャハンドルの設定
	void SetTextureHandle(const uint32_t& textureHandle);

	uint32_t GetTextureHandle() { return textureHandle_; }


	// 座標の設定
	void SetPosition(const Vector2& position);

	const Vector2& GetPosition() { return position_; }

	// 角度の設定
	void SetRotation(float rotation);

	float GetRotation() { return rotation_; }

	// サイズの設定
	void SetSize(const Vector2& size);
	void SetSizeX(const float& size);
	const Vector2& GetSize() { return size_; }

	// アンカーポイントの設定
	void SetAnchorPoint(const Vector2& anchorpoint);

	const Vector2& GetAnchorPoint() { return anchorPoint_; }

	// 色の設定
	void SetColor(const Vector4& color) { color_ = color; };

	const Vector4& GetColor() { return color_; }

	// 左右反転の設定
	void SetIsFlipX(const bool& isFlipX);
	bool GetIsFlipX() { return isFlipX_; }

	// 上下反転の設定
	void SetIsFlipY(bool isFlipY);
	bool GetIsFlipY() { return isFlipY_; }

	// テクスチャ範囲設定
	void SetTextureRect(const Vector2& texBase, const Vector2& texSize);

	void SetDrawUpdate(const Vector2& Position,const Vector4& Color);

	void SetRatio(const float& xRatio,const float& yRatio);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Vector2& Position,const Vector4& Color,const int& blendMode = 1);

private: // メンバ変数
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	VertexPosUv* vertMap_ = nullptr;
	ConstBufferData* constMap_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	UINT textureHandle_ = 0;
	float rotation_ = 0.0f;
	Vector2 position_{};
	Vector2 size_ = { 100.0f, 100.0f };
	Vector2 anchorPoint_ = { 0, 0 };
	Matrix4 matWorld_{};
	Vector4 color_ = { 1, 1, 1, 1 };
	bool isFlipX_ = false;//左右入れ替え
	bool isFlipY_ = false;//上下入れ替え
	Vector2 texBase_ = { 0, 0 };
	Vector2 texSize_ = { 100.0f, 100.0f };
	Vector2 ratio = { 1.0f,1.0f };
	D3D12_RESOURCE_DESC resourceDesc_ = {};

private: // メンバ関数
	/// <summary>
	/// 頂点データ転送
	/// </summary>
	void TransferVertices();
};
