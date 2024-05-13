#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include "ViewProjection.h"
#include "TextureManager.h"


class Trail
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
protected:
	struct SwordTrailVertex
	{
		Vector3 pos = { 0, 0, 0 };
		Vector2 uv = { 0, 0 };
		Vector4 Color = { 1,1,1,1 };
	};
public:
	struct PosBuffer
	{
		Vector3 head; //先端の位置
		Vector3 tail; //末端の位置
	};
	struct colorBuffer
	{
		Vector4 color;
	};
protected:
	std::vector<PosBuffer> posArray_;
	std::vector<SwordTrailVertex> vertex_;
	uint32_t vertSize_;
	//現在フレームでの位置
	PosBuffer tempPos;

	uint32_t TexNum;

	bool isVisible_ = true;
	bool isStop_ = true;

	bool isStartColor = false;
	bool isEndColor = false;

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_;

	//色情報
	ComPtr<ID3D12Resource> constBuffColor_;
	//色情報
	colorBuffer* constMapColor_;

	Vector4 color_;

	Vector3 FirstColor_;
	Vector3 EndColor_;

private:
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;
	// パイプラインステートオブジェクト
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelineState_;

public:
	// 静的初期化
	static void StaticInitialize();
	// グラフィックスパイプラインの初期化
	static void InitializeGraphicsPipeline();

public:
	Trail(uint32_t vertSize);
	void Update();
	void SetPos(const Vector3& head, const Vector3& tail);
	void SetIsVisible(bool flag) { isVisible_ = flag; }
	void SetColor(const Vector4& color) { color_ = color; }
	void SetTexture(const uint32_t& texNum_);
	void PreDraw();
	void Draw(const ViewProjection& view);

	void ResetTrail(const Vector3& resetPos = { 0,0,0 });

	void SetFirstColor(const Vector3& color);
	void SetEndColor(const Vector3& color);

private:
	//データ転送
	void TransferBuff();
	void CreateCurveVertex(std::vector<PosBuffer>& usedPosArray);
public:
	Vector4 GetColor() { return color_; }
};

