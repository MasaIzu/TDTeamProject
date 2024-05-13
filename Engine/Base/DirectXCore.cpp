#include "DirectXCore.h"

MY_SUPPRESS_WARNINGS_BEGIN

#include <algorithm>
#include <cassert>
#include <vector>
MY_SUPPRESS_WARNINGS_END

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using namespace Microsoft::WRL;

DirectXCore* DirectXCore::DirectXCore_ = nullptr;

DirectXCore* DirectXCore::GetInstance() {
	if (DirectXCore_ == nullptr)
	{
		DirectXCore_ = new DirectXCore();
	}

	return DirectXCore_;
}

DirectXCore::~DirectXCore() {

	dxgiFactory.Reset();
	commandList.Reset();
	commandAllocator.Reset();
	commandQueue.Reset();
	swapChain.Reset();
	backBuffers.clear();
	depthBuffer.Reset();
	rtvHeap.Reset();
	dsvHeap.Reset();
	fence.Reset();

	device.Reset();

}

void DirectXCore::Destroy(){

	delete DirectXCore_;
}

void DirectXCore::DirectXCoreInitialize(const HWND& hwnd, const float& window_width, const float& window_height) {

	backBufferWidth_ = window_width;
	backBufferHeight_ = window_height;

	hwnd_ = hwnd;

	// デバイス初期化
	InitializeDevice();

	// コマンド関連初期化
	InitializeCommand();

	// スワップチェーンの生成
	InitializeSwapChain();

	// レンダーターゲット生成
	InitializeRenderTargets();

	// 深度バッファ生成
	InitializeDepthBuffer();

	// フェンス生成
	InitializeFence();
}

void DirectXCore::PreDraw() {
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	// リソースバリアを変更（表示状態→描画対象）
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->ResourceBarrier(1, &barrier);

	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeap->GetCPUDescriptorHandleForHeapStart(), static_cast<INT>(bbIndex),
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// レンダーターゲットをセット
	commandList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// 全画面クリア
	ClearRenderTarget();
	// 深度バッファクリア
	ClearDepthBuffer();

	// ビューポートの設定
	CD3DX12_VIEWPORT viewport =
		CD3DX12_VIEWPORT(0.0f, 0.0f, float(backBufferWidth_), float(backBufferHeight_));
	commandList->RSSetViewports(1, &viewport);
	// シザリング矩形の設定
	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, static_cast<LONG>(backBufferWidth_), static_cast<LONG>(backBufferHeight_));
	commandList->RSSetScissorRects(1, &rect);
}

void DirectXCore::PostDraw() {
	HRESULT result;

	// リソースバリアを変更（描画対象→表示状態）
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);
	commandList->ResourceBarrier(1, &barrier);

	// 命令のクローズ
	commandList->Close();

	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { commandList.Get() }; // コマンドリストの配列
	commandQueue->ExecuteCommandLists(1, cmdLists);

	// バッファをフリップ
	result = swapChain->Present(1, 0);
#ifdef _DEBUG
	if (FAILED(result)) {
		ComPtr<ID3D12DeviceRemovedExtendedData> dred;

		result = device->QueryInterface(IID_PPV_ARGS(&dred));
		assert(SUCCEEDED(result));

		// 自動パンくず取得
		D3D12_DRED_AUTO_BREADCRUMBS_OUTPUT autoBreadcrumbsOutput{};
		result = dred->GetAutoBreadcrumbsOutput(&autoBreadcrumbsOutput);
		assert(SUCCEEDED(result));
	}
#endif

	// コマンドリストの実行完了を待つ
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		if ( event != 0 )
		{
			WaitForSingleObject(event,INFINITE);
			CloseHandle(event);
		}
	}

	commandAllocator->Reset(); // キューをクリア
	commandList->Reset(commandAllocator.Get(),
		nullptr); // 再びコマンドリストを貯める準備
}

void DirectXCore::ClearRenderTarget() {
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeap->GetCPUDescriptorHandleForHeapStart(),static_cast<INT>(bbIndex),
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	// 全画面クリア        Red   Green Blue  Alpha
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 0.0f }; // 青っぽい色
	commandList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
}

void DirectXCore::ClearDepthBuffer() {
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	commandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}


void DirectXCore::InitializeDevice() {
	HRESULT result = S_FALSE;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	//デバッグレイヤーをオンに
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
	// DREDレポートをオンに
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings> dredSettings;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&dredSettings)))) {
		dredSettings->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dredSettings->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
	}
#endif

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] = {
	  D3D_FEATURE_LEVEL_12_1,
	  D3D_FEATURE_LEVEL_12_0,
	  D3D_FEATURE_LEVEL_11_1,
	  D3D_FEATURE_LEVEL_11_0,
	};

	// DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter;
	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(
		i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&tmpAdapter)) !=
		DXGI_ERROR_NOT_FOUND;
		i++) {
		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}

	// ハードウェアアダプタを優先的に処理
	std::stable_sort(
		adapters.begin(), adapters.end(),
		[](const ComPtr<IDXGIAdapter4>& lhs, const ComPtr<IDXGIAdapter4>& rhs) {
			DXGI_ADAPTER_DESC3 lhsDesc;
			lhs->GetDesc3(&lhsDesc); // アダプターの情報を取得
			DXGI_ADAPTER_DESC3 rhsDesc;
			rhs->GetDesc3(&rhsDesc); // アダプターの情報を取得
			return (lhsDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE) <
				(rhsDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE);
		});

	result = S_FALSE;
	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < adapters.size(); i++) {
		// デバイスを生成
		for (int levelIndex = 0; levelIndex < _countof(levels); levelIndex++) {
			result =
				D3D12CreateDevice(adapters[i].Get(), levels[levelIndex], IID_PPV_ARGS(&device));
			if (SUCCEEDED(result)) {
				// デバイスを生成できた時点でループを抜ける
				featureLevel = levels[levelIndex];
				break;
			}
		}

		// このアダプタで生成できてたら完了
		if (SUCCEEDED(result)) {
			break;
		}
	}

	assert(!!device);
	assert(SUCCEEDED(result));

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		// 抑制するエラー
		D3D12_MESSAGE_ID denyIds[] = {
			
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE };
		// 抑制する表示レベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// 指定したエラーの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
		// エラー時にブレークを発生させる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	}
#endif
}

void DirectXCore::InitializeSwapChain() {
	HRESULT result = S_FALSE;

	// 各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = static_cast<UINT>(backBufferWidth_);
	swapChainDesc.Height = static_cast<UINT>(backBufferHeight_);
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色情報の書式を一般的なものに
	swapChainDesc.SampleDesc.Count = 1;                // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファとして使えるように
	swapChainDesc.BufferCount = 2;                      // バッファ数を２つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は速やかに破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING; // ティアリングサポート
	ComPtr<IDXGISwapChain1> swapChain1;
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(), hwnd_, &swapChainDesc, nullptr, nullptr, &swapChain1);
	assert(SUCCEEDED(result));

	// SwapChain4を得る
	swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain));
	assert(SUCCEEDED(result));

	// OSが行うAlt+Enterのフルスクリーンは制御不能なので禁止
	dxgiFactory->MakeWindowAssociation(
		hwnd_, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
}

void DirectXCore::InitializeCommand() {
	HRESULT result = S_FALSE;

	// コマンドアロケータを生成
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(result));

	// コマンドリストを生成
	result = device->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

	// 標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
}

void DirectXCore::InitializeRenderTargets() {
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapChain->GetDesc(&swcDesc);
	assert(SUCCEEDED(result));

	// 各種設定をしてディスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	heapDesc.NumDescriptors = swcDesc.BufferCount;
	result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap));
	assert(SUCCEEDED(result));

	// 裏表の２つ分について
	backBuffers.resize(swcDesc.BufferCount);
	for (size_t i = 0; i < backBuffers.size(); i++) {
		// スワップチェーンからバッファを取得
		result = swapChain->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(&backBuffers[i]));
		assert(SUCCEEDED(result));

		// ディスクリプタヒープのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			rtvHeap->GetCPUDescriptorHandleForHeapStart(),static_cast<UINT>(i),
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		// レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		// シェーダーの計算結果をSRGBに変換して書き込む
		renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		device->CreateRenderTargetView(backBuffers[i].Get(), &renderTargetViewDesc, handle);
	}
}

void DirectXCore::InitializeDepthBuffer() {
	HRESULT result = S_FALSE;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	// リソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT, static_cast<UINT>(backBufferWidth_), static_cast<UINT>(backBufferHeight_), 1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	CD3DX12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	// リソースの生成
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値書き込みに使用
		&clearValue, IID_PPV_ARGS(&depthBuffer));
	assert(SUCCEEDED(result));

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;                    // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビュー
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	assert(SUCCEEDED(result));

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCore::InitializeFence() {
	HRESULT result = S_FALSE;

	// フェンスの生成
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(result));
}
