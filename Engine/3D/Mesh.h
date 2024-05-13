#pragma once
#include "Material.h"
#include "DirectXCore.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#include "Defined.h"

MY_SUPPRESS_WARNINGS_BEGIN
#include <d3d12.h>
#include <d3dx12.h>
#include <Windows.h>
#include <unordered_map>
#include <vector>
#include <wrl.h>
MY_SUPPRESS_WARNINGS_END

// ノード
struct Node
{
	//名前
	std::string name;
	//ローカル変形行列
	Matrix4 transform;
	//グローバル変形行列
	Matrix4 globalTransform;
	//親ノード
	Node* parent = nullptr;
	//子ノード
	std::vector<Node*>childrens;

};

/// <summary>
/// メッシュデータ
/// </summary>
class Mesh {
	//friend class FbxLoader;
	//friend class FbxModel;

public:
	// 骨
	struct Bone
	{
		//名前
		std::string name;

		Matrix4 matrix;
		Matrix4 animationMatrix;
		Matrix4 offsetMatirx;

		UINT index;

	};

private: // エイリアス
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public: // サブクラス
	static const int MAX_BONE_INDICES = 4;
	// 頂点データ構造体（テクスチャあり）
	struct VertexPosNormalUv {
		Vector3 pos;    // xyz座標
		Vector3 normal; // 法線ベクトル
		Vector2 uv;     // uv座標

		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];

	};

public: // メンバ関数
	/// <summary>
	/// 名前を取得
	/// </summary>
	/// <returns>名前</returns>
	const std::string& GetName() { return name_; }

	/// <summary>
	/// 名前をセット
	/// </summary>
	/// <param name="name">名前</param>
	void SetName(const std::string& name_);

	/// <summary>
	/// 頂点データの追加
	/// </summary>
	/// <param name="vertex">頂点データ</param>
	void AddVertex(const VertexPosNormalUv& vertex);

	/// <summary>
	/// 頂点インデックスの追加
	/// </summary>
	/// <param name="index">インデックス</param>
	void AddIndex(const unsigned short& index);

	/// <summary>
	/// 頂点データの数を取得
	/// </summary>
	/// <returns>頂点データの数</returns>
	inline size_t GetVertexCount() { return vertices_.size(); }

	/// <summary>
	/// エッジ平滑化データの追加
	/// </summary>
	/// <param name="indexPosition">座標インデックス</param>
	/// <param name="indexVertex">頂点インデックス</param>
	void AddSmoothData(const unsigned short& indexPosition,const unsigned short& indexVertex);

	/// <summary>
	/// 平滑化された頂点法線の計算
	/// </summary>
	void CalculateSmoothedVertexNormals();

	/// <summary>
	/// マテリアルの取得
	/// </summary>
	/// <returns>マテリアル</returns>
	Material* GetMaterial() { return material_; }

	/// <summary>
	/// マテリアルの割り当て
	/// </summary>
	/// <param name="material">マテリアル</param>
	void SetMaterial(Material* material);


	//void SetLight(float alpha);

	void SetLight(const Vector3& ambient,const Vector3& diffuse,const Vector3& specular,const float& alpha);

	/// <summary>
	/// バッファの生成
	/// </summary>
	void CreateBuffers();

	/// <summary>
	/// 頂点バッファ取得
	/// </summary>
	/// <returns>頂点バッファ</returns>
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView_; }

	/// <summary>
	/// インデックスバッファ取得
	/// </summary>
	/// <returns>インデックスバッファ</returns>
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView_; }

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList">命令発行先コマンドリスト</param>
	/// <param name="rooParameterIndexMaterial">マテリアルのルートパラメータ番号</param>
	/// <param name="rooParameterIndexTexture">テクスチャのルートパラメータ番号</param>
	void Draw(
		ID3D12GraphicsCommandList* commandList,const UINT& rooParameterIndexMaterial,
		const UINT& rooParameterIndexTexture,const uint32_t& textureIndex);

	void Draw(
		ID3D12GraphicsCommandList* commandList,const UINT& rooParameterIndexMaterial,
		const UINT& rooParameterIndexTexture,const uint32_t& textureIndex,const uint32_t& shadowMapTextureIndex);


	/// <summary>
	/// 描画（テクスチャ差し替え版）
	/// </summary>
	/// <param name="commandList">命令発行先コマンドリスト</param>
	/// <param name="rooParameterIndexMaterial">マテリアルのルートパラメータ番号</param>
	/// <param name="rooParameterIndexTexture">テクスチャのルートパラメータ番号</param>
	/// <param name="textureHandle">差し替えるテクスチャハンドル</param>
	/*void Draw(
		ID3D12GraphicsCommandList* commandList, UINT rooParameterIndexMaterial,
		UINT rooParameterIndexTexture, uint32_t textureHandle);*/

	/// <summary>
	/// 頂点配列を取得
	/// </summary>
	/// <returns>頂点配列</returns>
	inline const std::vector<VertexPosNormalUv>& GetVertices() { return vertices_; }

	/// <summary>
	/// インデックス配列を取得
	/// </summary>
	/// <returns>インデックス配列</returns>
	inline const std::vector<unsigned short>& GetIndices() { return indices_; }

private: // メンバ変数
	// 名前
	std::string name_;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_;

	//ボーンバッファ
	ComPtr<ID3D12Resource> BoneBuff_;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices_;
	// 頂点インデックス配列
	std::vector<unsigned short> indices_;
	// 頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;
	// マテリアル
	Material* material_ = nullptr;

	std::unordered_map<std::string, Bone*> bones;

	std::vector<Bone> vecBones;

	Node* node = nullptr;


	// マッピング済みアドレス
	Bone* constMap = nullptr;
};