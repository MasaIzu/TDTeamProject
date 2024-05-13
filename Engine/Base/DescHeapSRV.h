#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "TextureMaterial.h"

/// <summary>
/// SRV用デスクリプタヒープ
/// </summary>
class DescHeapSRV
{
private: //エイリアス
	//Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//テクスチャの最大枚数
	static const int SRVCount = 512;

public: //静的メンバ関数
	/// <summary>
	/// 共通初期化設定
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="cmdList">コマンドリスト</param>
	static void Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 全シーン共通で使用するテクスチャ枚数を確定させる
	/// </summary>
	static void SetAllSceneTextureNum();

	/// <summary>
	/// シーン切り替え時に全シーン共通テクスチャ以外の分インデックスを戻す
	/// </summary>
	static void TextureDestruction();

	/// <summary>
	/// SRV共通SetDescriptorHeaps
	/// </summary>
	static void SetDescriptorHeaps();

	/// <summary>
	/// SRV共通CreateShaderResourceView
	/// </summary>
	static void CreateShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc,TextureMaterial& texture);

	/// <summary>
	/// SRV共通SetGraphicsRootDescriptorTable
	/// </summary>
	static void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texNumber);

public: //静的メンバ変数  (たくさん使うので、使いやすいようにpublicにしておく)
	//デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeapSRV;

private: //静的メンバ変数
	//デバイス
	static ID3D12Device* dev;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//全シーン共通で使用するテクスチャの枚数
	static UINT allSceneTextureNum;
	//テクスチャ番号カウンター
	static UINT texNumCount;
};