#pragma once

#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Mesh.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <string>
#include <unordered_map>
#include <vector>
MY_SUPPRESS_WARNINGS_END

/// <summary>
/// コンピュートシェーダー実装用
/// </summary>
class ParticleCS
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
	//// 頂点データ構造体
	//struct VertexPosNormalUv
	//{
	//	Vector3 pos; // xyz座標
	//	Vector3 normal; // 法線ベクトル
	//	vextor2 uv;  // uv座標
	//};

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
		bool alive = 0; // このパーティクルが生きているかどうか
		float MinusAlpha = 0.0f;
		float DownScale = 0;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Matrix4 mat;	// 3D変換行列
		Matrix4 matBillboard;//ビルボード行列
		UINT maxParticleCount;
		UINT particleCount;
		Vector3 StartPos;
	};

	struct GpuParticleElement {
		Vector3 position;
		float scale = 1;
		Vector4 color = { 1,1,1,1 };
		UINT  isActive;	// 生存フラグ.
		float lifeTime;
		float elapsed;
		UINT  colorIndex;
		Vector4 velocity;
	};

	struct ShaderParameters {
		Vector4 StartPos;
		Matrix4 mat;	// 3D変換行列
		Matrix4 matBillboard;//ビルボード行列
		UINT maxParticleCount;
		UINT particleCount;
	};
	ShaderParameters shaderParameters;

	//// パーティクルの定義
	//struct Particle {
	//	Vector3 position;
	//	Vector3 velocity;
	//	int Frame; // このパーティクルが生まれたフレーム
	//	int MaxFrame;//このパーティクルの寿命
	//	bool alive; // このパーティクルが生きているかどうか
	//	float scale = 1;
	//	int a = 0;
	//	Vector4 color = { 1,1,1,1 };
	//};


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
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

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

	static std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> m_pipelines;

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertPaticleStatus;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// アップロードバッファの作成
	ComPtr<ID3D12Resource> uploadBuffer;
	ComPtr<ID3D12Resource> m_cacheVertexBuffer;

	ComPtr<ID3D12Resource1> m_sceneParameterCB;
	ComPtr<ID3D12Resource1> m_gpuParticleIndexList;
	ComPtr<ID3D12Resource1> m_gpuParticleElement;
	static ComPtr<ID3D12DescriptorHeap> m_cbvSrvUavHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE processedCommandsHandle_;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_handleCpu;
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_handleGpu;
	static UINT m_incrementSize;

	static const std::string PSO_DEFAULT;
	static const std::string PSO_CS_INIT;
	static const std::string PSO_CS_EMIT;
	static const std::string PSO_CS_UPDATE;
	static const std::string PSO_DRAW_PARTICLE;
	static const std::string PSO_DRAW_PARTICLE_USE_TEX;

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
	/// <param name="ParticleCount">どのくらい作るのか</param>
	void Initialize(uint32_t ParticleCount);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// コンピュートシェーダーアップデート
	/// </summary>
	void CSUpdate(ID3D12GraphicsCommandList* cmdList,Vector4 StartPos);

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

	//コンピュートシェーダー掛けた後のコピー処理
	void CopyData();




private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ


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

	uint32_t numParticles = 50000;
	uint32_t MaxParticleCount = 50000;

	// パーティクルデータをアップロードバッファにコピー
	void* mappedData = nullptr;

	// Create a fence.
	ComPtr<ID3D12Fence> fence;
	uint64_t fenceValue = 0;
	UINT64 m_frameCount = 0;
	static UINT m_cbvSrvUavDescriptorSize;

	uint32_t particleCount;
};
