#include "ShadowMap.h"
#include "DescHeapSRV.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#include "TextureManager.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

ID3D12Device* ShadowMap::dev = nullptr;
ID3D12GraphicsCommandList* ShadowMap::cmdList = nullptr;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ShadowMap::descHeapSRV;
uint32_t ShadowMap::texNum = 0;

ShadowMap* ShadowMap::Create()
{
	//インスタンスを生成
	ShadowMap* instance = new ShadowMap();
	if ( instance == nullptr )
	{
		return nullptr;
	}

	// 初期化
	if ( !instance->Initialize() )
	{
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

void ShadowMap::ShadowMapCommon(ID3D12Device* dev_,ID3D12GraphicsCommandList* cmdList_)
{
	//nullptrチェック
	assert(dev_);
	assert(cmdList_);

	ShadowMap::dev = dev_;
	ShadowMap::cmdList = cmdList_;
}

void ShadowMap::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* cmdList_,const uint32_t& rootParaIndex)
{
	// SRVをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(cmdList_,rootParaIndex,texNum);
}

bool ShadowMap::Initialize()
{
	HRESULT result;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(( sizeof(ConstBufferData) * 0xff ) & ~0xff);

	//定数バッファの生成
	result = dev->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			shadowMapTexSize,
			shadowMapTexSize,
			1,0,
			1,0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES depthHeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE depthResourceDesc = CD3DX12_CLEAR_VALUE( DXGI_FORMAT_D32_FLOAT,1.0f,0 );

	//深度バッファの生成
	result = dev->CreateCommittedResource(
		&depthHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&depthResourceDesc,
		IID_PPV_ARGS(&textureMaterial.texBuff)
	);
	assert(SUCCEEDED(result));

	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープを生成
	result = dev->CreateDescriptorHeap(&DescHeapDesc,IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	//デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(textureMaterial.texBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV作成
	texNum = TextureManager::CreateShaderResourceView(srvDesc,textureMaterial.texBuff);
	

	return true;
}

void ShadowMap::DrawScenePrev()
{
	TextureManager::Trans(cmdList,texNum,D3D12_RESOURCE_STATE_GENERIC_READ,D3D12_RESOURCE_STATE_DEPTH_WRITE);


	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(0,nullptr,false,&dsvH);

	CD3DX12_VIEWPORT viewport = CD3DX12_VIEWPORT(0.0f,0.0f,shadowMapTexSize,shadowMapTexSize);

	//ビューポートの設定
	cmdList->RSSetViewports(1,&viewport);


	CD3DX12_RECT rect = CD3DX12_RECT(0,0,shadowMapTexSize,shadowMapTexSize);

	//シザリング矩形の設定
	cmdList->RSSetScissorRects(1,&rect);

	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH,D3D12_CLEAR_FLAG_DEPTH,1.0f,0,0,
		nullptr);
}

void ShadowMap::DrawSceneRear()
{
	TextureManager::Trans(cmdList,texNum,D3D12_RESOURCE_STATE_DEPTH_WRITE,D3D12_RESOURCE_STATE_GENERIC_READ);
}
