#include "PostEffectManager.h"


void PostEffectManager::Initialize(DirectXCore* dxCore, const uint32_t& WinWidth, const uint32_t& WinHeight)
{
	PostEffect::Initialize(dxCore, WinWidth, WinHeight);

	MixPostEffect::Initialize(dxCore, WinWidth, WinHeight);

	RadialBlurPostEffect::Initialize(dxCore,WinWidth,WinHeight);
}

void PostEffectManager::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	MixPostEffect::PreDrawScene(cmdList);
}

void PostEffectManager::PostDrawScene()
{
	MixPostEffect::PostDrawScene();
}

void PostEffectManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	MixPostEffect::Draw(cmdList);
}

void PostEffectManager::Finalize()
{
	PostEffect::Finalize();
	MixPostEffect::Finalize();
	RadialBlurPostEffect::Finalize();
}
