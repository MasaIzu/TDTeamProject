#pragma once

#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Mesh.h"
#include "LightGroup.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "MyStruct.h"
#include "TextureMaterial.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <string>
#include <unordered_map>
#include <vector>
MY_SUPPRESS_WARNINGS_END

/// <summary>
/// モデルデータ
/// </summary>
class Model {
private:
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public:

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Vector4 color;	// 色 (RGBA)
		Matrix4 mat;	// ３Ｄ変換行列
	};

	struct VertexPos {
		Vector3 pos;    // xyz
	};

private:
	static const std::string kBaseDirectory;
	static const std::string kDefaultModelName;

private: // 静的メンバ変数
	// デスクリプタサイズ
	static UINT sDescriptorHandleIncrementSize_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;
	// パイプラインステートオブジェクト
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelineState_;
	// ライト
	static std::unique_ptr<LightGroup> lightGroup;

	//シャドウ用
	//パイプラインステートオブジェクト
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> LightViewPipelinestate;
	//ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> LightViewRootsignature;

public: // 静的メンバ関数
	// 静的初期化
	static void StaticInitialize();

	// 静的初期化
	static void StaticFinalize();

	// グラフィックスパイプラインの初期化
	static void InitializeGraphicsPipeline();
	static void CreateLightViewPipeline();


	// 3Dモデル生成
	static Model* Create();

	// OBJファイルからメッシュ生成
	static Model* CreateFromOBJ(const std::string& modelname,const bool& smoothing = false);

	// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	// 描画前処理
	static void PreShadowDraw(ID3D12GraphicsCommandList* commandList);
	static void PostShadowDraw();

	// 描画後処理
	static void PostDraw();

	static void SetShadowMapTexture(const TextureMaterial& shadowMapTexture_) {
		shadowMapTexture = shadowMapTexture_;
	};

public: // メンバ関数
	// デストラクタ
	~Model();

	// 初期化
	void Initialize(const std::string& modelname, bool smoothing = false);

	// 描画
	void Draw(
		const WorldTransform& worldTransform, const ViewProjection& viewProjection,const ViewProjection& lightViewProjection);

	void ShadowDraw(const WorldTransform& worldTransform,const ViewProjection& viewProjection);


	void Draw(
		const WorldTransform& worldTransform,const ViewProjection& viewProjection,
		const uint32_t& textureHadle);

	// メッシュコンテナを取得
	inline const std::vector<Mesh*>& GetMeshes() { return meshes_; }

	/// <summary>
	/// 頂点配列を取得
	/// </summary>
	/// <returns>頂点配列</returns>
	inline const std::vector<Mesh::VertexPosNormalUv>& GetVertices() { return vertices; }

	/// <summary>
	/// インデックス配列を取得
	/// </summary>
	/// <returns>インデックス配列</returns>
	inline const std::vector<unsigned short>& GetIndices() { return indices; }

	/// <summary>
	/// テクスチャのセット
	/// </summary>
	/// <param name="textureHandle">テクスチャ</param>
	void SetTextureHandle(const uint32_t& textureHandle) { modelTextureHandle = textureHandle; }

	/// <summary>
	/// 三角形頂点だけ算出
	/// </summary>
	/// <returns></returns>
	std::vector<VertexPos> GetMeshVertice() { return onlyTriangleVertices; }
	/// <summary>
	/// メッシュパーティクルよう
	/// </summary>
	/// <returns></returns>
	std::vector<MyStruct::Meshes> GetMeshData() { return mesheData; }
	/// <summary>
	/// 名前
	/// </summary>
	std::string GetName();

private: // メンバ変数
	// 名前
	std::string name_;
	// 名前
	std::string exportName_;
	// メッシュコンテナ
	std::vector<Mesh*> meshes_;
	// マテリアルコンテナ
	std::unordered_map<std::string, Material*> materials_;
	// デフォルトマテリアル
	Material* defaultMaterial_ = nullptr;

	//頂点データ
	std::vector<Mesh::VertexPosNormalUv>vertices;
	std::vector<MyStruct::Meshes>mesheData;
	std::vector<VertexPos>onlyTriangleVertices;

	//頂点インデックス
	std::vector<unsigned short> indices;

	uint32_t modelTextureHandle = 0;

	//影用深度テクスチャ
	static TextureMaterial shadowMapTexture;

private: // メンバ関数

	// モデル読み込み
	void LoadModel(const std::string& modelname, bool smoothing);

	// マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	// マテリアル登録
	void AddMaterial(Material* material);

	// テクスチャ読み込み
	void LoadTextures();
};
