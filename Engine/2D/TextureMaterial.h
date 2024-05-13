#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>

/// <summary>
/// テクスチャ材料
/// </summary>
struct TextureMaterial
{
private: //エイリアス 
	//Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //メンバ変数
	//テクスチャリソース(テクスチャバッファ)
	ComPtr<ID3D12Resource> texBuff;
	//テクスチャ番号
	uint32_t texNumber = 0;
};