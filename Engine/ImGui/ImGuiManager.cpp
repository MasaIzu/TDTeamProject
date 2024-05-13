#include "ImGuiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>


void ImGuiManager::Initialize(WinApp* winApp, DirectXCore* dxcore)
{
	HRESULT result;

	dxcore_ = dxcore;
	assert(dxcore_);

	//ImGuiのコンテキストを生成
	ImGui::CreateContext();
	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(winApp->Gethwnd());

	//デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//デスクリプタヒープ生成
	result = dxcore_->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	ImGui_ImplDX12_Init(
		dxcore_->GetDevice(),
		static_cast<int>(dxcore_->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);

}

void ImGuiManager::Finalize()
{
	//後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//デスクリプタヒープの開放
	srvHeap_.Reset();
}

void ImGuiManager::Bigin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* commandlist = dxcore_->GetCommandList();

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	commandlist->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandlist);
}