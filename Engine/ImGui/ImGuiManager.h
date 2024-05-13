#pragma once

#include "WinApp.h"

#include "DirectXCore.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <d3dx12.h>
#include <imgui.h>
MY_SUPPRESS_WARNINGS_END

//ImGUIの管理
class ImGuiManager
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp, DirectXCore* dxcommon);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Bigin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 画面への描画
	/// </summary>
	void Draw();

private:
	DirectXCore* dxcore_ = nullptr;

	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

};