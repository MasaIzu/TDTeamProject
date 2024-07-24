#include "Framework.h"
#include "MixPostEffect.h"
#include <PostEffectManager.h>
#include <Sprite3D.h>
#include <FbxLoader.h>
#include <FBXObject3d.h>
#include "LightData.h"
#include "NumbersSprite.h"
#include "RadialBlurPostEffect.h"
#include "DescHeapSRV.h"
#include "Trail.h"
#include "Trail3D.h"
#include <ParticleEditor.h>

uint32_t Framework::Time = 0;

void Framework::Initialize()
{

	// ゲームウィンドウの作成
	winApp_ = WinApp::GetInstance();
	winApp_->MakeWindow(L"Game");

	// DirectX初期化処理
	directXCore_ = DirectXCore::GetInstance();
	directXCore_->DirectXCoreInitialize(winApp_->Gethwnd(), winApp_->GetWindowSize().x, winApp_->GetWindowSize().y);

#pragma region 汎用機能初期化

	// 入力の初期化
	input_ = Input::GetInstance();
	input_->Initialize();

	//SRV用デスクリプタヒープの初期化
	DescHeapSRV::Initialize(directXCore_->GetDevice(),directXCore_->GetCommandList());

	// テクスチャマネージャの初期化
	TextureManager_ = TextureManager::GetInstance();
	TextureManager_->Initialize(directXCore_->GetDevice());
	TextureManager::Load("white1x1.png");

	//ライト静的初期化
	LightGroup::StaticInitialize(directXCore_->GetDevice());
	LightData::GetInstance()->StaticInitialize();

	//シャドウマップ共通初期化処理
	ShadowMap::ShadowMapCommon(directXCore_->GetDevice(),directXCore_->GetCommandList());
	//シャドウマップの初期化
	shadowMap.reset(ShadowMap::Create());

	// FBX関連静的初期化
	FbxLoader::GetInstance()->Initialize(directXCore_->GetDevice());
	// デバイスをセット
	FBXObject3d::SetDevice(directXCore_->GetDevice());
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();
	FBXObject3d::CreateLightGraphicsPipeline();

	// スプライト静的初期化
	Sprite::StaticInitialize(directXCore_->GetDevice());
	Sprite3D::StaticInitialize(directXCore_->GetDevice());

	// 3Dモデル静的初期化
	Model::StaticInitialize();
	Model::SetShadowMapTexture(shadowMap->GetTexture());
	/*FbxModel::StaticInitialize();*/

	//Imgui初期化
	imGui = std::make_unique <ImGuiManager>();
	imGui->Initialize(winApp_, directXCore_);

	fps = std::make_unique<FPS>();

	ParticleEditor::StaticInitialize(directXCore_->GetDevice());

	//ポストエフェクトの初期化処理
	PostEffectManager::Initialize(DirectXCore::GetInstance(), winApp_->window_width, winApp_->window_height);

	NumbersSprite::GetInstance()->Initialize();

	//トレイルシェーダー初期化
	Trail::StaticInitialize();
	Trail3D::StaticInitialize();
#pragma endregion

	//シーンマネージャーの生成
	sceneManager_ = SceneManager::GetInstance();


}

void Framework::Update()
{
	//fps制限
	fps->FpsControlBegin();

	// メッセージ処理
	if (winApp_->ProcessMessage()) {
		isEndRequst = true;
	}

	// 入力関連の毎フレーム処理
	input_->Update();

	//Imguiの更新
	imGui->Bigin();

	//デモウィンドウの表示オン
	//ImGui::ShowDemoWindow();

	//シーンマネージャーの更新
	sceneManager_->Update();

	//Imguiのコマンド受付終了
	imGui->End();
}

void Framework::Finalize()
{
	// 各種解放
	sceneManager_->Finalize();

	NumbersSprite::GetInstance()->Destroy();

	PostEffectManager::Finalize();

	imGui->Finalize();
	sceneFactory_.reset();

	Model::StaticFinalize();

	Sprite::StaticFinalize();
	Sprite3D::StaticFinalize();
	LightData::GetInstance()->Destroy();
	TextureManager_->Delete();

	input_->Destroy();

	directXCore_->Destroy();

	// ゲームウィンドウの破棄
	winApp_->DeleteGameWindow();
	winApp_->Destroy();


}

bool Framework::isPlayMyGame()
{
	return isEndRequst;
}

bool Framework::isGameSlow()
{
	return isSlow;
}

void Framework::Run()
{

	//ゲームの初期化
	Initialize();

	while (true) {//ゲームループ

		if ( isFirstUpdate )
		{
			isFirstUpdate = false;
			//毎フレーム更新
			Update();
			Time++;
		}
		else
		{
			Update();
			if ( !IsSlow() )
			{
				//Time = 0;
			}
			else
			{
				Time++;
			}
		}
		//終了リクエストが来たら抜ける
		if (isPlayMyGame()) {
			break;
		}

		CSUpdate();

		//SRV用共通デスクリプタヒープSetDescriptorHeaps
		//DescHeapSRV::SetDescriptorHeaps();

		//シャドウマップのレンダーテクスチャへの描画
		shadowMap->DrawScenePrev();
		sceneManager_->BackgroundDraw();
		shadowMap->DrawSceneRear();

		ID3D12GraphicsCommandList* commandList = directXCore_->GetCommandList();

		PostEffect::PreDrawScene(commandList);
		Draw();
		PostEffect::PostDrawScene();

		//RadialBlurPostEffect::PreDrawScene(commandList);

		//RadialBlurPostEffect::PostDrawScene();

		//PostEffectManager::PreDrawScene(directXCore_->GetCommandList());

		//PostEffect::PreDrawScene(directXCore_->GetCommandList(),false);
		//PostEffect::Draw(directXCore_->GetCommandList());
		//PostEffect::PostDrawScene();

		PostEffect::PreDrawScene(commandList,false);
		PostEffect::SetShadeNumber(3);
		PostEffectDraw();
		PostEffect::PostDrawScene();

		PostEffectManager::PreDrawScene(directXCore_->GetCommandList());
		Draw();
		PostEffect::Draw(directXCore_->GetCommandList(),0);
		PostEffectManager::PostDrawScene();

		//RadialBlurPostEffect::SetShadeNumber(2);
		//RadialBlurPostEffect::Draw(directXCore_->GetCommandList(),0);

		//PostEffectManager::PostDrawScene();


		// 描画開始
		directXCore_->PreDraw();

		PostEffectManager::Draw(directXCore_->GetCommandList());

		//ImGui描画
#ifdef _Editor
		imGui->Draw();
#elif _DEBUG
		imGui->Draw();

#endif

		// 描画終了
		directXCore_->PostDraw();

		//FPS固定
		fps->FpsControlEnd();

		if ( IsBreak() )
		{
			break;
		}

		if ( input_->TriggerKey(DIK_ESCAPE) )
		{
			break;
		}

	}

	//ゲームの終了
	Finalize();


}
