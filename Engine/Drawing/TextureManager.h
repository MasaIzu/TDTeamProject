#pragma once


#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <array>
#include <d3dx12.h>
#include <string>
#include <unordered_map>
#include <wrl.h>
MY_SUPPRESS_WARNINGS_END

/// <summary>
/// テクスチャマネージャー
/// </summary>
class TextureManager {
public:
	// デスクリプターの数
	static const size_t kNumDescriptors = 256;

	/// <summary>
	/// テクスチャ
	/// </summary>
	struct Texture {
		// テクスチャリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		// シェーダリソースビューのハンドル(CPU)
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		// シェーダリソースビューのハンドル(CPU)
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		// 名前
		std::string name;
	};

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>テクスチャハンドル</returns>
	static uint32_t Load(const std::string& fileName);

	/// <summary>
	/// シャドウ用のクリエイト
	/// </summary>
	/// <param name="srvDesc"></param>
	/// <param name="texBuff"></param>
	/// <param name="descHeapSRV"></param>
	/// <returns></returns>
	static 	uint32_t CreateShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc,Microsoft::WRL::ComPtr<ID3D12Resource>& texBuff);


	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static TextureManager* GetInstance();


	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <returns>デストラクタ</returns>
	void Delete();

	/// <summary>
	/// システム初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(ID3D12Device* device,const std::string& directoryPath = "Resources/");

	/// <summary>
	/// 全テクスチャリセット
	/// </summary>
	void ResetAll();

	/// <summary>
	/// リソース情報取得
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <returns>リソース情報</returns>
	const D3D12_RESOURCE_DESC GetResoureDesc(const uint32_t& textureHandle);

	/// <summary>
	/// デスクリプタテーブルをセット
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rootParamIndex">ルートパラメータ番号</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SetGraphicsRootDescriptorTable(
		ID3D12GraphicsCommandList* commandList,const UINT& rootParamIndex,const uint32_t& textureHandle);

	static void SetDescriptorHeap(ID3D12GraphicsCommandList* commandList);

	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap();

	static CD3DX12_RESOURCE_BARRIER Trans(ID3D12GraphicsCommandList* commandList,const uint32_t& texNum,D3D12_RESOURCE_STATES stateBefore,D3D12_RESOURCE_STATES stateAfter);

private:
	static TextureManager* TextureManager_;

private:
	TextureManager() = default;
	~TextureManager();
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	// デバイス
	ID3D12Device* device_ = nullptr;
	// デスクリプタサイズ
	UINT DescriptorHandleSize_ = 0u;
	// ディレクトリパス
	std::string directoryPath_;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	// 次に使うデスクリプタヒープの番号
	uint32_t NextDescriptorHeapNumber_ = 0u;
	// テクスチャコンテナ
	std::array<Texture, kNumDescriptors> textures_;

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	uint32_t LoadInternal(const std::string& fileName);

	/// <summary>
	/// シャドウ用
	/// </summary>
	/// <param name="srvDesc"></param>
	/// <param name="texBuff"></param>
	/// <param name="descHeapSRV"></param>
	/// <returns></returns>
	uint32_t CreateShaderResourceViewInternal(const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc,Microsoft::WRL::ComPtr<ID3D12Resource>& texBuff);

	void SetDescriptorHeaps(ID3D12GraphicsCommandList* commandList);

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeaps();

	CD3DX12_RESOURCE_BARRIER TransInternal(ID3D12GraphicsCommandList* commandList,const uint32_t& texNum,D3D12_RESOURCE_STATES stateBefore,D3D12_RESOURCE_STATES stateAfter);
};
