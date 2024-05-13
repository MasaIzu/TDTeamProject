#pragma once
#include"DirectXCore.h"
#include "Vector3.h"
#include "Vector2.h"
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
/// 実際にポストエフェクトをかける方
/// </summary>
class PostEffect
{
public:
	 
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

    //ポストエフェクトに関するGPUに送りたいものまとめ
    struct SendDataToGPU {
        int shadeNumber;
        int kernelSize;
        Vector2 center;
        float intensity;
        int samples;
        float _AngleDeg;
        float _AngleDeg2;
    };

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize(DirectXCore* dxCore,const uint32_t& WinWidth,const uint32_t& WinHeight);

	/// <summary>
	/// 修正処理
	/// </summary>
	static void Finalize();
    
    /// <summary>
    /// パイプライン生成
    /// </summary>
    static void CreatGraphicsPipelineState();

    /// <summary>
    /// シーン描画前処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    static void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画
	/// </summary>
	static void Draw(ID3D12GraphicsCommandList* cmdList,const uint32_t blendNum = 0);

    /// <summary>
    /// シーン描画後処理
    /// </summary>
    /// <param name="cmdList">コマンド処理</param>
    static void PostDrawScene();


	/// <summary>
	/// シェーダーナンバーセット
	/// </summary>
	static void SetShadeNumber(const int& SetShadeNumber);

	/// <summary>
	/// レンジの大きさ
	/// </summary>
	static void SetKernelSize(const int& range);

	/// <summary>
	/// ラジアルブラー
	/// </summary>
	static void SetRadialBlur(const Vector2& senter,const float& intensity,const int& sample);

	/// <summary>
	/// アングル変更
	/// </summary>
	static void SetAngle(const float& angle1,const float& angle2);



private://静的メンバ変数
    static const float clearColor[4];

    static ID3D12Device* device_;

    static ID3D12GraphicsCommandList* commandList;

    static VertexPosUv vertices[4];

    static VertexPosUv* vertMap;

    static Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;	//頂点バッファ

    //頂点バッファビューの作成
    static D3D12_VERTEX_BUFFER_VIEW vbView;
    static Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[2];

    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //深度バッファ
    static Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    //RTV用のデスクリプタヒープ
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用のデスクリプタヒープ
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>,2>PipelineStates_;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
private:
    // 定数バッファ
    static Microsoft::WRL::ComPtr<ID3D12Resource> constDataBuff_;
    // マッピング済みアドレス
    static SendDataToGPU* dataMap;
};

