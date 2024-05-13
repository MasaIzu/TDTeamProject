#include"LightGroup.h"

using namespace DirectX;

/// <summary>
///静的メンバ変数の実態
/// </summary>
ID3D12Device* LightGroup::device = nullptr;

void LightGroup::StaticInitialize(ID3D12Device* device_)
{
	//最初期化チェック
	assert(!LightGroup::device);

	//nullチェック
	assert(device_);

	LightGroup::device = device_;

}

LightGroup* LightGroup::Create()
{
	LightGroup* instance = new LightGroup();

	instance->Initialize();

	return instance;
}

void LightGroup::Initialize()
{
	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;                   // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;   // 256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	/*ID3D12Resource* constBuff = nullptr;*/
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

void LightGroup::Update()
{
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}

void LightGroup::ClearLight()
{
	pointLights.clear();
	spotLights.clear();
	circleShadows.clear();
}

void LightGroup::TransferConstBuffer()
{
	//定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->ambientColor = ambientColor;

		constMap->pointLightCount = static_cast< uint32_t >( pointLights.size() );
		constMap->spotLightCount = static_cast< uint32_t >( spotLights.size() );
		constMap->circleShadowCount = static_cast< uint32_t >( circleShadows.size() );

		//点ライト
		for (int i = 0; i < pointLights.size(); i++) {
			if (pointLights[i].IsActive()) {
				constMap->pointLights[i].active = 1;
				constMap->pointLights[i].lightpos = pointLights[i].GetLightPos();
				constMap->pointLights[i].lightcolor = pointLights[i].GetLightColor();
				constMap->pointLights[i].lightatten = pointLights[i].GetLightAtten();
			}
			else {
				constMap->pointLights[i].active = 0;
			}
		}
		//スポットライト
		for (int i = 0; i < spotLights.size(); i++) {
			//ライトが有効なら設定を転送
			if (spotLights[i].IsActive()) {
				constMap->spotLights[i].active = 1;
				constMap->spotLights[i].lightv = -spotLights[i].GetLightDir();
				constMap->spotLights[i].lightpos = spotLights[i].GetLightPos();
				constMap->spotLights[i].lightcolor = spotLights[i].GetLightColor();
				constMap->spotLights[i].lightatten = spotLights[i].GetLightAtten();
				constMap->spotLights[i].lightfactoranglecos = spotLights[i].GetLightFactorAngleCos();
			}
			else {//ライトが無効ならライトを0に
				constMap->spotLights[i].active = 0;
			}
		}
		//丸影
		for (int i = 0; i < circleShadows.size(); i++) {
			//有効なら設定を転送
			if (circleShadows[i].IsActive()) {
				constMap->circleShadows[i].active = 1;
				constMap->circleShadows[i].dir = -circleShadows[i].GetDir();
				constMap->circleShadows[i].casterPos = circleShadows[i].GetCasterPos();
				constMap->circleShadows[i].distanceCasterLight = circleShadows[i].GetDistanceCasterLight();
				constMap->circleShadows[i].atten = circleShadows[i].GetAtten();
				constMap->circleShadows[i].factorAngleCos = circleShadows[i].GetFactorAngleCos();
			}
			else {
				constMap->circleShadows[i].active = 0;
			}
		}

		constBuff->Unmap(0, nullptr);
	}

}
