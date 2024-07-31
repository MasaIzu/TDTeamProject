#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include "ViewProjection.h"
#include "TextureManager.h"


class Trail3D
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
		float Size = 0.0f;
	};
public:
	struct PosBuffer
	{
		Vector3 position;
	};
	struct Material
	{
		Matrix4 matBillboard;//ビルボード行列
	};
protected:
	std::vector<PosBuffer> posArray_;
	std::vector<SwordTrailVertex> vertex_;
	uint32_t vertSize_;
	//現在フレームでの位置
	PosBuffer tempPos;

	uint32_t TexNum;
	uint32_t backcount = 2;
	bool isVisible_ = true;
	bool isStop_ = true;

	bool isStartColor = false;
	bool isEndColor = false;

	bool isFirstPointSize = false;
	bool isSenterPointSize = false;
	bool isEndPointSize = true;

	bool isAlphaDown = false;

	size_t alphaTime = 0;
	size_t MaxAlphaTime = 0;

	float alpha = 1.0f;
	float MaxSize = 3.0f;
	float MinSize = 2.0f;

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_;

	//色情報
	ComPtr<ID3D12Resource> constBuffColor_;
	//色情報
	Material* constMapMaterial_;

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
	Trail3D(uint32_t vertSize);
	void Update();
	void SetPos(const Vector3& pos);
	void SetIsVisible(bool flag) { isVisible_ = flag; }
	void SetColor(const Vector4& color) { color_ = color; }
	void SetTexture(const uint32_t& texNum_);
	void PreDraw();
	void Draw(const ViewProjection& view);

	void ResetTrail(const Vector3& resetPos = { 0,0,0 },const size_t& time = 0);

	void SetFirstColor(const Vector3& color);
	void SetEndColor(const Vector3& color);

	void SetSizeWeightPoint(const bool& first,const bool& center,const bool& end);

private:
	//データ転送
	void TransferBuff();
	void CreateCurveVertex(std::vector<PosBuffer>& usedPosArray);
public:
	Vector4 GetColor() { return color_; }
};

