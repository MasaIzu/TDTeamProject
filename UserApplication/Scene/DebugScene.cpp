#include "DebugScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
//#include "FbxLoader.h"
#include"ImGuiManager.h"
#include <CollisionAttribute.h>
#include "Collision.h"
#include"PostEffect.h"
#include"WinApp.h"
#include <LightData.h>

#include <iostream>
#include <filesystem>

DebugScene::DebugScene() {}

DebugScene::~DebugScene() {
	LightData::GetInstance()->ClearLight();
}

// 指定ディレクトリのファイルリストを取得
std::vector<std::string> getFileList(const std::filesystem::path& directory) {
	std::vector<std::string> fileList;
	for ( const auto& entry : std::filesystem::directory_iterator(directory) )
	{
		if ( entry.is_regular_file() )
		{
			fileList.push_back(entry.path().filename().string());
		}
	}
	return fileList;
}

void DebugScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();
	viewProjection_->eye = { 0,5,-100 };
	viewProjection_->UpdateMatrix();

	LightViewProjection = std::make_unique<ViewProjection>();
	LightViewProjection->Initialize();
	LightViewProjection->eye = { -200,400,-800 };
	LightViewProjection->UpdateMatrix();

	debugCamera = std::make_unique<DebugCamera>();
	debugCamera->Initialize(viewProjection_.get());

	AddShield_ = std::make_unique<AddShield>();

	uint32_t MaxParticleCountB = 1000000;
	particleEditor = std::make_unique<ParticleEditor>();
	particleEditor->SetAddShield(AddShield_.get());
	particleEditor->Initialize(MaxParticleCountB, true);
	particleEditor->SetAnimationTextureHandle(TextureManager::Load("sprite/effect4.png"),10);

}

void DebugScene::Update() {
	particleEditor->EditUpdate();


	if (!particleEditor->GetIsWindowFocus())
	{
		debugCamera->Update();
	}

	LightData::GetInstance()->Update();
}

void DebugScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	PostEffect::PreDrawScene(commandList);
	PostEffect::SetShadeNumber(shadeNumber);
	PostEffect::SetKernelSize(range);
	PostEffect::SetRadialBlur(center, intensity, samples);
	PostEffect::SetAngle(angle, angle2);

	Model::PreDraw(commandList);

	if (particleEditor->IsStageDraw())
	{
		AddShield_->Draw(*viewProjection_.get(), *LightViewProjection.get());
	}

	Model::PostDraw();

	Model::PreDraw(commandList);

	Model::PostDraw();

	particleEditor->Draw(*viewProjection_);

	PostEffect::PostDrawScene();
}

void DebugScene::BackgroundDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Model::PreShadowDraw(commandList);//// 3Dオブジェクト描画前処理
	AddShield_->ShadowDraw(*LightViewProjection.get());
	Model::PostShadowDraw();
}

void DebugScene::CSUpdate()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	particleEditor->CSUpdate(commandList);
}

bool DebugScene::IsBreak()
{
	return isFinishGame;
}

bool DebugScene::IsSlow()
{
	return isSlowGame;
}

void DebugScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();


#pragma endregion

#pragma region 3Dオブジェクト描画

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	//3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画



#pragma endregion
}

void DebugScene::Finalize()
{
}
