#pragma once
#include <DirectXMath.h>
#include "TextureMaterial.h"

/// <summary>
/// シャドウマップ
/// </summary>
class ShadowMap
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color;	//色(RGBA)
		XMMATRIX mat;	//3変換行列
	};

public: //静的メンバ関数
	/// <summary>
	/// シャドウマップ生成
	/// </summary>
	/// <returns>シャドウマップ</returns>
	static ShadowMap* Create();

	/// <summary>
	/// シャドウマップ共通部分の初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="cmdList">コマンドリスト</param>
	static void ShadowMapCommon(ID3D12Device* dev,ID3D12GraphicsCommandList* cmdList);

	static void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* cmdList,const uint32_t& rootParaIndex);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize();

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	void DrawScenePrev();

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	void DrawSceneRear();

	//getter
	const TextureMaterial& GetTexture() {
		return textureMaterial;
	}

private: //静的メンバ変数
	//デバイス
	static ID3D12Device* dev;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//シャドウマップテクスチャサイズ(4K)
	static const int shadowMapTexSize = 4096;
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	static uint32_t texNum;

private: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//テクスチャ材料
	TextureMaterial textureMaterial;

	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;


};

