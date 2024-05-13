#pragma once
#include "PostEffect.h"
#include "MixPostEffect.h"
#include "RadialBlurPostEffect.h"

/// <summary>
/// ポストエフェクトマネージャー
/// </summary>
class PostEffectManager {
public:

	//初期化
    static void Initialize(DirectXCore* dxCore, const uint32_t& WinWidth, const uint32_t& WinHeight);

    /// <summary>
    /// シーン描画前処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    static void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// シーン描画後処理
    /// </summary>
    /// <param name="cmdList">コマンド処理</param>
    static void PostDrawScene();
	//描画
    static void Draw(ID3D12GraphicsCommandList* cmdList);
	//終了処理
    static void Finalize();

private:




};