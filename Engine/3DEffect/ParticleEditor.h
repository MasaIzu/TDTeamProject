#pragma once

#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Mesh.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#include "MyStruct.h"

#include "AddShield.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include <iostream>

#include <string>
#include <unordered_map>
#include <vector>
MY_SUPPRESS_WARNINGS_END

/// <summary>
/// ブーストパーティクル
/// </summary>
class ParticleEditor
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス

	struct BoostPos
	{
		Vector4 StartPos;

	};

	// 頂点データ構造体
	struct VertexPos
	{
		Vector3 position;
		float scale = 1;
		Vector4 color = { 1,1,1,1 };
		Vector4 DownColor = { 0,0,0,0 };
		Vector3 velocity;
		Vector3 FinalVelocity;
		uint32_t Frame = 0; // このパーティクルが生まれたフレーム
		uint32_t MaxFrame = 0;//このパーティクルの寿命
		uint32_t alive = 0; // このパーティクルが生きているかどうか
		float MinusAlpha = 0.0f;
		float DownScale = 0;
	};

	struct GpuParticleElement
	{
		Vector4 position;
		Vector4 color;
		Vector4 velocity;
		uint32_t isActive;
		float lifeTime;
		float MaxLifeTime;
		float scale;
		float Speed;
		float graceOfTime;
		float ScaleKeep;
		float GroupNumber;
		float GroupTimer;
		float MaxGroupTimer;
		float PostEffectPow;
	};

	struct ShaderViewParameters
	{
		Matrix4 mat;	// 3D変換行列
		Matrix4 matBillboard;//ビルボード行列
	};
	ShaderViewParameters shaderViewParameters;

	struct ShaderDetailPointGenerationParameters
	{
		Vector4 StartPos = { 0,0,0,0 };
		Vector4 EndPos = { 0,50,0,0 };
		Vector4 StartColor = { 1,1,1,1 };
		Vector4 EndColor = { 1,1,1,1 };
		Vector4 Angle = { 0,0,0,0 };
		uint32_t Shot = 0;
		uint32_t EndPointActive = 0;
		uint32_t RandomVelocity = 1;
		uint32_t RandomLife = 0;
		uint32_t RandomSpeed = 0;
		uint32_t RandomScale = 0;
		float Speed = 1.0f;
		float LerpStrength = 0.05f;
		float Scale = 1.0f;
		float ScaleTinker = 0.0f;
		float MaxLife = 60.0f;
		uint32_t MaxParticleCount = 0;
		uint32_t AdditiveSynthesis = 0;
		uint32_t isLoad = 0;
		Vector2 RandomLifeMinMax = { 1,30 };
		Vector2 RandomSpeedMinMax = { 1,10 };
		Vector2 RandomScaleMinMax = { 1,10 };
		float SpeedDivideSize = 1;
		float ScaleDivideSize = 1;
		float GravityStrength = 0;
		uint32_t Interlocking = 0;
		float InterlockingStrength = 0;
		float InterlockingLength = 15;
		uint32_t ScaleDownLifeTime = 0;
		uint32_t EmitParticles = 0;
		uint32_t ParticleGroup = 0;
		uint32_t ParticleGroupCount = 5;
		float ExplosionTimer = 30;
		float MaxExplosionTimer = 300;
		Vector2 RandomExplosionTimerMinMax = { 1,30 };
		uint32_t RandomParticleExplosion = false;
		uint32_t ShapeNumber = 0;
		float Width = 1.0f;
		float Height = 1.0f;
		float Depth = 1.0f;
		uint32_t CollisionON = 0;
		Vector4 ShapeScale = { 1,1,1,1 };
		Vector3 VelocityAdjustment = { 1,1,1 };
		uint32_t GettingUpDownScale = 0;
		float PostEffectPow = 0.0f;
	};
	ShaderDetailPointGenerationParameters shaderDetailParameters;

	struct SendPointGenerationParameters
	{
		float StartPos[ 4 ] = { 0,0,0,0 };
		float EndPos[ 4 ] = { 0,50,0,0 };
		float StartColor[ 4 ] = { 1,1,1,1 };
		float EndColor[ 4 ] = { 1,1,1,1 };
		float Angle[ 4 ] = { 0,0,0,0 };
		bool Shot = 0;
		bool EndPointActive = 0;
		bool RandomVelocity = 1;
		bool RandomLife = 0;
		bool RandomSpeed = 0;
		bool RandomScale = 0;
		float Speed = 1.0f;
		float LerpStrength = 0.05f;
		float Scale = 1.0f;
		float ScaleTinker = 0.0f;
		float MaxLife = 60.0f;
		uint32_t MaxParticleCount = 0;
		bool AdditiveSynthesis = 0;
		uint32_t isLoad = 0;
		float RandomLifeMinMax[ 2 ] = { 1,30 };
		float RandomSpeedMinMax[ 2 ] = { 1,10 };
		float RandomScaleMinMax[ 2 ] = { 1,10 };
		float SpeedDivideSize = 1;
		float ScaleDivideSize = 1;
		float GravityStrength = 0;
		bool Interlocking = false;
		float InterlockingStrength = 0;
		float InterlockingLength = 15;
		bool ScaleDownLifeTime = false;
		bool EmitParticles = false;
		bool ParticleGroup = false;
		int ParticleGroupCount = 5;
		float ExplosionTimer = 30;
		float MaxExplosionTimer = 300;
		float RandomExplosionTimerMinMax[ 2 ] = { 1,30 };
		bool RandomParticleExplosion = false;
		uint32_t ShapeNumber = 0;
		float Width = 1.0f;
		float Height = 1.0f;
		float Depth = 1.0f;
		float ShapeScale[ 4 ] = { 1,1,1,1 };
		bool CollisionON = false;
		bool GettingUpDownScale = false;
		float VelocityAdjustment[ 3 ] = { 1,1,1 };
		float PostEffectPow = 0.0f;
	};
	SendPointGenerationParameters sendParameters;

	static const uint32_t MaxColCount = 30;

	struct ShaderDetailCollision
	{
		Vector4 Pos[ MaxColCount ];
		Vector4 Scale[ MaxColCount ];
		uint32_t ColCount = 0;
	};
	static ShaderDetailCollision shaderDetailCollision_;

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize(ID3D12Device* device);


	/// <summary>
	/// 静的終了処理
	/// </summary>
	static void StaticFinalize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList,const bool& isAddition = false);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// コリジョンの追加
	/// </summary>
	/// <param name="colPos">コリジョンの追加</param>
	static uint32_t AddCollision(const Vector3& colPos,const Vector3& colScale);

	/// <summary>
	/// コリジョンの変更
	/// </summary>
	/// <param name="colPos">コリジョンの追加</param>
	static void ChangeCollision(const uint32_t& colcount,const Vector3& colPos,const Vector3& colScale);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// コンピュートシェーダー用ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// コンピュートシェーダー用パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelineState;

	static std::unordered_map<std::string,ComPtr<ID3D12PipelineState>> m_pipelines;

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertPaticleStatus;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// アップロードバッファの作成
	ComPtr<ID3D12Resource> uploadBuffer;
	ComPtr<ID3D12Resource> m_cacheVertexBuffer;

	ComPtr<ID3D12Resource1> m_sceneViewParameterCB;
	ComPtr<ID3D12Resource1> m_sceneDetailParameterCB;
	ComPtr<ID3D12Resource1> m_gpuParticleIndexList;
	ComPtr<ID3D12Resource1> m_gpuParticleElement;
	static ComPtr<ID3D12DescriptorHeap> m_cbvSrvUavHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE processedCommandsHandle_;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_handleCpu;
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_handleGpu;
	static UINT m_incrementSize;

	static const std::string PSO_DEFAULT;
	static const std::string PSO_ADD;
	static const std::string PSO_CS_INIT;
	static const std::string PSO_CS_EMIT;
	static const std::string PSO_CS_UPDATE;
	static const std::string PSO_DRAW_PARTICLE;
	static const std::string PSO_DRAW_PARTICLE_USE_TEX;

	static ComPtr<ID3D12Resource1> m_sceneDetailCollisionParameterCB;

private:// 静的メンバ関数

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// 頂点バッファの初期化
	/// </summary>
	void InitializeVerticeBuff();



public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const uint32_t& ParticleCount,const bool& isEditUpdate_ = false,const std::string& FileName = "Nothing");

	/// <summary>
	/// 遮蔽のセット
	/// </summary>
	void SetAddShield(AddShield* AddShield);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& FileName);

	/// <summary>
	/// エディターアップデート
	/// </summary>
	void EditUpdate();

	/// <summary>
	/// コンピュートシェーダーアップデート
	/// </summary>
	void CSUpdate(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// コンピュートシェーダーアップデート
	/// </summary>
	void CSUpdate(ID3D12GraphicsCommandList* commandList,const Vector4& StartPos,const uint32_t& isParticleActive);

	/// <summary>
	/// コンピュートシェーダーアップデート
	/// </summary>
	void CSUpdate(ID3D12GraphicsCommandList* commandList,const uint32_t& isParticleActive,const Vector4& EndPos);

	/// <summary>
	/// コンピュートシェーダーアップデート
	/// </summary>
	void CSUpdate(ID3D12GraphicsCommandList* commandList,const uint32_t& isParticleActive,const uint32_t& isEndPointActive,const Vector4& EndPos);

	/// <summary>
	/// コンピュートシェーダーアップデート
	/// </summary>
	void CSUpdate(ID3D12GraphicsCommandList* commandList,const uint32_t& isParticleActive,const uint32_t& isEndPointActive,const Vector4& EndPos,const float& speed);

	/// <summary>
	/// コンピュートシェーダーアップデート
	/// </summary>
	void CSUpdate(ID3D12GraphicsCommandList* commandList,const Vector4& StartPos,const Vector4& EndPos);

	/// <summary>
	/// コンピュートシェーダーアップデート
	/// </summary>
	void CSUpdate(ID3D12GraphicsCommandList* commandList,const Vector4& StartPos,const Vector4& EndPos,const uint32_t& isParticleActive);

	void CSCmd(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& view);


	/// <summary>
	/// テクスチャのセット
	/// </summary>
	void SetTextureHandle(uint32_t textureHandle);

	/// <summary>
	/// 全体のサイズ
	/// </summary>
	size_t GetParticlesListSize() {
		return Particles.size();
	}

	ShaderDetailPointGenerationParameters GetShaderDetailParameters() const { return shaderDetailParameters; }

	/// <summary>
	/// 最初の色のゲット
	/// </summary>
	Vector4 GetFirstColorParticle();

	//コンピュートシェーダー掛けた後のコピー処理
	void CopyData();

	//ステージ描画するか
	bool IsStageDraw();

	bool GetIsWindowFocus();

private:
	std::vector<std::string> GetFileList(const std::filesystem::path& directory);

	void SetParameter();
	void LoadFileParameter(const SendPointGenerationParameters& params);

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ

	AddShield* addShield;

	//// 色
	//Vector4 color = { 1,1,1,1 };
	//// ローカルスケール
	//Vector3 scale = { 1,1,1 };
	//// X,Y,Z軸回りのローカル回転角
	//Vector3 rotation = { 0,0,0 };
	//// ローカル座標
	//Vector3 position = { 0,0,0 };
	// ローカルワールド変換行列
	Matrix4 matWorld;
	//// 親オブジェクト
	//ParticleManager* parent = nullptr;

	//パーティクル配列
	std::vector<VertexPos>Particles;

	UINT textureHandle_ = 0;

	// パーティクルデータをアップロードバッファにコピー
	void* mappedData = nullptr;

	// Create a fence.
	ComPtr<ID3D12Fence> fence;
	uint64_t fenceValue = 0;
	UINT64 m_frameCount = 0;
	static UINT m_cbvSrvUavDescriptorSize;
	static uint32_t ParticleEdiCount;

	bool isPushSave = false;
	bool isPushLoad = false;
	bool isPushReset = false;
	bool isCreateNewFile = false;
	bool isDeletFileFirstTime = false;
	bool isDeletFileSecondTime = false;
	bool isSetNewParticleCount = false;
	bool isGravityStrengthActive = false;
	bool isEditUpdate = false;
	bool isLoadFile = false;
	bool isStageDraw = false;
	bool isParticleActiveCheckBox = false;
	bool isWindowFocus = false;
	bool isPostEffctActive = false;
	bool isMakeCol = false;
	bool isSet = false;

	int selectedIndex = -1;  // 選択されている項目のインデックス
	int NewParticleCount = 0;
	int particleLifeMaxCount = 50;

	uint32_t particleCount;
	uint32_t particleEdiCount = 0;

	float AngleX_ = 0.0f;
	float AngleY_ = 0.0f;
	float AngleZ_ = 0.0f;
	float GravityStrength = 0.0f;
	float GravityStrengthDiv = 1.0f;

	float ColPos[ 3 ] = { 0,0,0 };
	float ColScale[ 3 ] = { 1,1,1 };

	float StartColor[ 4 ] = { 1,1,1,1 };
	float EndColor[ 4 ] = { 1,1,1,1 };
	float Angle[ 4 ] = { 0,0,0,0 };

	std::string FilePath = "Resources/ParticleData/";
	std::string SelectedFileName;
	std::string NewFileName;
	std::string KeepFileName;

	//リストボックス用
	// シェイプの選択肢を保持する配列
	const char* shapes[ 4 ] = { "Point", "Box", "Triangle", "Circle" };
	// 選択されたシェイプのインデックス（デフォルトは0: "Point"）
	int selectedItem = 0;

};
