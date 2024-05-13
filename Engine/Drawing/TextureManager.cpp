#include "TextureManager.h"
#include <DirectXTex.h>
#include <cassert>


using namespace DirectX;
TextureManager* TextureManager::TextureManager_ = nullptr;

uint32_t TextureManager::Load(const std::string& fileName) {
	return TextureManager::GetInstance()->LoadInternal(fileName);
}

TextureManager* TextureManager::GetInstance() {
	if (TextureManager_ == nullptr)
	{
		TextureManager_ = new TextureManager();
	}

	return TextureManager_;
}

TextureManager::~TextureManager()
{
	descriptorHeap_.Reset();
}

void TextureManager::Delete()
{
	delete TextureManager_;
}

void TextureManager::Initialize(ID3D12Device* device,const std::string& directoryPath) {
	assert(device);

	device_ = device;
	directoryPath_ = directoryPath;

	// デスクリプタサイズを取得
	DescriptorHandleSize_ =
	  device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// 全テクスチャリセット
	ResetAll();
}

void TextureManager::ResetAll() {
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダから見えるように
	descHeapDesc.NumDescriptors = kNumDescriptors; // シェーダーリソースビュー1つ
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptorHeap_)); // 生成
	assert(SUCCEEDED(result));

	NextDescriptorHeapNumber_ = 0;

	// 全テクスチャを初期化
	for (size_t i = 0; i < kNumDescriptors; i++) {
		textures_[i].resource.Reset();
		textures_[i].cpuDescHandleSRV.ptr = 0;
		textures_[i].gpuDescHandleSRV.ptr = 0;
		textures_[i].name.clear();
	}
}

const D3D12_RESOURCE_DESC TextureManager::GetResoureDesc(const uint32_t& textureHandle) {

	assert(textureHandle < textures_.size());
	Texture& texture = textures_.at(textureHandle);
	return texture.resource->GetDesc();
}

void TextureManager::SetGraphicsRootDescriptorTable(
  ID3D12GraphicsCommandList* commandList,const UINT& rootParamIndex,
  const uint32_t& textureHandle) { // デスクリプタヒープの配列
	assert(textureHandle < textures_.size());
	ID3D12DescriptorHeap* ppHeaps[] = {descriptorHeap_.Get()};
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// シェーダリソースビューをセット
	commandList->SetGraphicsRootDescriptorTable(rootParamIndex, textures_[textureHandle].gpuDescHandleSRV);

}

void TextureManager::SetDescriptorHeap(ID3D12GraphicsCommandList* commandList)
{
	TextureManager::GetInstance()->SetDescriptorHeaps(commandList);
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> TextureManager::GetDescriptorHeap()
{
	return TextureManager::GetInstance()->GetDescriptorHeaps();
}

CD3DX12_RESOURCE_BARRIER TextureManager::Trans(ID3D12GraphicsCommandList* commandList,const uint32_t& texNum,D3D12_RESOURCE_STATES stateBefore,D3D12_RESOURCE_STATES stateAfter)
{
	return TextureManager::GetInstance()->TransInternal(commandList,texNum,stateBefore,stateAfter);
}


uint32_t TextureManager::LoadInternal(const std::string& fileName) {

	assert(NextDescriptorHeapNumber_ < kNumDescriptors);
	uint32_t handle = NextDescriptorHeapNumber_;

	// 読み込み済みテクスチャを検索
	auto it = std::find_if(textures_.begin(), textures_.end(), [&](const auto& texture) {
		return texture.name == fileName;
	});
	if (it != textures_.end()) {
		// 読み込み済みテクスチャの要素番号を取得
		handle = static_cast<uint32_t>(std::distance(textures_.begin(), it));
		return handle;
	}

	// 書き込むテクスチャの参照
	Texture& texture = textures_.at(handle);
	texture.name = fileName;

	// ディレクトリパスとファイル名を連結してフルパスを得る
	bool currentRelative = false;
	if (2 < fileName.size()) {
		currentRelative = (fileName[0] == '.') && (fileName[1] == '/');
	}
	std::string fullPath = currentRelative ? fileName : directoryPath_ + fileName;

	// ユニコード文字列に変換
	wchar_t wfilePath[256];
	MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilePath, _countof(wfilePath));

	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// WICテクスチャのロード
	result = LoadFromWICFile(wfilePath, WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));

	ScratchImage mipChain{};
	// ミップマップ生成
	result = GenerateMipMaps(
	  scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
	  TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	  metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize,
	  (UINT16)metadata.mipLevels);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps =
	  CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	// テクスチャ用バッファの生成
	result = device_->CreateCommittedResource(
	  &heapProps, D3D12_HEAP_FLAG_NONE, &texresDesc,
	  D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
	  nullptr, IID_PPV_ARGS(&texture.resource));
	assert(SUCCEEDED(result));

	// テクスチャバッファにデータ転送
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0); // 生データ抽出
		result = texture.resource->WriteToSubresource(
		  (UINT)i,
		  nullptr,              // 全領域へコピー
		  img->pixels,          // 元データアドレス
		  (UINT)img->rowPitch,  // 1ラインサイズ
		  (UINT)img->slicePitch // 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	// シェーダリソースビュー作成
	texture.cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(
	  descriptorHeap_->GetCPUDescriptorHandleForHeapStart(), handle, DescriptorHandleSize_);
	texture.gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(
	  descriptorHeap_->GetGPUDescriptorHandleForHeapStart(), handle, DescriptorHandleSize_);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texture.resource->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = (UINT)metadata.mipLevels;

	device_->CreateShaderResourceView(
	  texture.resource.Get(), //ビューと関連付けるバッファ
	  &srvDesc,               //テクスチャ設定情報
	  texture.cpuDescHandleSRV);

	NextDescriptorHeapNumber_++;

	return handle;
}

uint32_t TextureManager::CreateShaderResourceViewInternal(const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc,Microsoft::WRL::ComPtr<ID3D12Resource>& texBuff)
{
	uint32_t handle = NextDescriptorHeapNumber_;

	// 書き込むテクスチャの参照
	Texture& texture = textures_.at(handle);
	texture.name = "shadowMap";

	// シェーダリソースビュー作成
	texture.cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(
	  descriptorHeap_->GetCPUDescriptorHandleForHeapStart(),handle,DescriptorHandleSize_);
	texture.gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(
	  descriptorHeap_->GetGPUDescriptorHandleForHeapStart(),handle,DescriptorHandleSize_);

	device_->CreateShaderResourceView(
	  texBuff.Get(), //ビューと関連付けるバッファ
	  &srvDesc,               //テクスチャ設定情報
	  CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeap_->GetCPUDescriptorHandleForHeapStart(),handle,
		  DescriptorHandleSize_
	  ));

	texture.resource = texBuff;

	NextDescriptorHeapNumber_++;

	return handle;
}

void TextureManager::SetDescriptorHeaps(ID3D12GraphicsCommandList* commandList)
{
	ID3D12DescriptorHeap* ppHeaps[ ] = { descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps),ppHeaps);
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> TextureManager::GetDescriptorHeaps()
{
	return descriptorHeap_;
}

CD3DX12_RESOURCE_BARRIER TextureManager::TransInternal(ID3D12GraphicsCommandList* commandList,const uint32_t& texNum,D3D12_RESOURCE_STATES stateBefore,D3D12_RESOURCE_STATES stateAfter)
{
	CD3DX12_RESOURCE_BARRIER transitionBarrier;
	transitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(textures_[ texNum ].resource.Get(),stateBefore,stateAfter);
	
	//リソースバリアを変更(シェーダリソース→描画可能)
	commandList->ResourceBarrier(1,&transitionBarrier);
	return transitionBarrier;
}

uint32_t TextureManager::CreateShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc,Microsoft::WRL::ComPtr<ID3D12Resource>& texBuff)
{
	return TextureManager::GetInstance()->CreateShaderResourceViewInternal(srvDesc,texBuff);
}
