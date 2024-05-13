#include "DirectXCore.h"
#include "Model.h"
#include <algorithm>
#include <cassert>
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>
#include <LightData.h>
#include <DescHeapSRV.h>
#include "ShadowMap.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const std::string Model::kBaseDirectory = "Resources/object/";
const std::string Model::kDefaultModelName = "cube";
UINT Model::sDescriptorHandleIncrementSize_ = 0;
ID3D12GraphicsCommandList* Model::sCommandList_ = nullptr;
ComPtr<ID3D12RootSignature> Model::sRootSignature_;
ComPtr<ID3D12PipelineState> Model::sPipelineState_;
std::unique_ptr<LightGroup> Model::lightGroup;
TextureMaterial Model::shadowMapTexture;
ComPtr<ID3D12PipelineState> Model::LightViewPipelinestate;
//ルートシグネチャ
ComPtr<ID3D12RootSignature> Model::LightViewRootsignature;

void Model::StaticInitialize() {

	// パイプライン初期化
	InitializeGraphicsPipeline();
	CreateLightViewPipeline();

	// ライト生成
	lightGroup.reset(LightGroup::Create());
}

void Model::StaticFinalize()
{

	sRootSignature_.Reset();
	sPipelineState_.Reset();

}

void Model::InitializeGraphicsPipeline() {
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;    // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト


	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/ObjVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(
			(char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/ObjPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(
			(char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	  {// xy座標(1行で書いたほうが見やすい)
	   "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	  {// 法線ベクトル(1行で書いたほうが見やすい)
	   "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	  {// uv座標(1行で書いたほうが見やすい)
	   "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	//  デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;                       // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0 = {};
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1,0);	//t0 レジスタ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV1 = {};
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1,1);	//t1 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[7];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[4].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[5].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[6].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプル
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&sRootSignature_));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = sRootSignature_.Get();

	// グラフィックスパイプラインの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
		&gpipeline, IID_PPV_ARGS(&sPipelineState_));
	assert(SUCCEEDED(result));
}

void Model::CreateLightViewPipeline()
{
	HRESULT result;

	ComPtr<ID3DBlob> vsBlob;		//頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;		//ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob;		//エラーオブジェクト

	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/ObjLightViewVS.hlsl",	//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//インクルード可能にする
		"main","vs_5_0",	//エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//デバック用設定
		0,
		&vsBlob,&errorBlob);

	if ( FAILED(result) )
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		//エラー内容を出力ウインドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/ObjLightViewPS.hlsl",	//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//インクルード可能にする
		"main","ps_5_0",	//エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//デバッグ用設定
		0,
		&psBlob,&errorBlob);

	if ( FAILED(result) )
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		//エラー内容を出力ウインドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//頂点レイアウト配列の宣言と設定
	D3D12_INPUT_ELEMENT_DESC inputLayout[ ] = {
		{	//xyz座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{	//法線ベクトル
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{	//uv座標
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	//パイプラインステートの設定変数の宣言と、各種項目の設定
	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[ 0 ];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//環境設定

	//共通設定
	blenddesc.BlendEnable = true;	//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;	//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う

	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;	//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-ソ

	//深度値フォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline.NumRenderTargets = 1;	//描画対象は1つ
	gpipeline.RTVFormats[ 0 ] = DXGI_FORMAT_R8G8B8A8_UNORM;	//0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1;	//1ピクセルにつき1回サンプリング

	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0 = {};
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1,0);	//t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[ 5 ];
	rootparams[ 0 ].InitAsConstantBufferView(0,0,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 1 ].InitAsConstantBufferView(1,0,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 2 ].InitAsConstantBufferView(2,0,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 3 ].InitAsDescriptorTable(1,&descRangeSRV0,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 4 ].InitAsConstantBufferView(3,0,D3D12_SHADER_VISIBILITY_ALL);

	//テクスチャサンプラーの設定
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init_1_0(_countof(rootparams),rootparams,1,&samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


	//ルートシグネチャの生成
	ComPtr<ID3DBlob> rootSigBlob;

	//バージョン自動判定でのシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1_0,&rootSigBlob,
		&errorBlob);

	//ルートシグネチャの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateRootSignature(0,rootSigBlob->GetBufferPointer(),rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&LightViewRootsignature));

	//グラフィックスパイプラインステートの生成
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = LightViewRootsignature.Get();

	//パイプラインステートの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&gpipeline,IID_PPV_ARGS(&LightViewPipelinestate));
}

Model* Model::Create() {
	// メモリ確保
	Model* instance = new Model;
	instance->Initialize(kDefaultModelName, false);

	return instance;
}

Model* Model::CreateFromOBJ(const std::string& modelname,const bool& smoothing) {
	// メモリ確保
	Model* instance = new Model;
	instance->Initialize(modelname, smoothing);

	return instance;
}

void Model::PreDraw(ID3D12GraphicsCommandList* commandList) {
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(Model::sCommandList_ == nullptr);

	// コマンドリストをセット
	sCommandList_ = commandList;

	// パイプラインステートの設定
	commandList->SetPipelineState(sPipelineState_.Get());
	// ルートシグネチャの設定
	commandList->SetGraphicsRootSignature(sRootSignature_.Get());
	// プリミティブ形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::PreShadowDraw(ID3D12GraphicsCommandList* commandList)
{
	// コマンドリストをセット
	sCommandList_ = commandList;
	//プリミティブ形状の設定コマンド(三角形リスト)
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//パイプラインステートとルートシグネチャの設定コマンド
	commandList->SetPipelineState(LightViewPipelinestate.Get());
	commandList->SetGraphicsRootSignature(LightViewRootsignature.Get());
}

void Model::PostShadowDraw()
{
	// コマンドリストを解除
	sCommandList_ = nullptr;
}

void Model::PostDraw() {
	// コマンドリストを解除
	sCommandList_ = nullptr;
}

Model::~Model() {
	for (auto m : meshes_) {
		delete m;
	}
	meshes_.clear();

	for (auto m : materials_) {
		delete m.second;
	}
	materials_.clear();
}

void Model::Initialize(const std::string& modelname, bool smoothing) {
	// モデル読み込み
	LoadModel(modelname, smoothing);
	exportName_ = modelname;
	// メッシュのマテリアルチェック
	for (auto& m : meshes_) {
		// マテリアルの割り当てがない
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial_ == nullptr) {
				// デフォルトマテリアルを生成
				defaultMaterial_ = Material::Create();
				defaultMaterial_->name_ = "no material";
				materials_.emplace(defaultMaterial_->name_, defaultMaterial_);
			}
			// デフォルトマテリアルをセット
			m->SetMaterial(defaultMaterial_);
		}
	}

	// メッシュのバッファ生成
	for (auto& m : meshes_) {
		m->CreateBuffers();
	}

	// マテリアルの数値を定数バッファに反映
	for (auto& m : materials_) {
		m.second->Update();
	}

	// テクスチャの読み込み
	LoadTextures();
}

void Model::LoadModel(const std::string& modelname, bool smoothing) {
	const string filename = modelname + ".obj";
	const string directoryPath = kBaseDirectory + modelname + "/";

	// ファイルストリーム
	std::ifstream file;
	// .objファイルを開く
	file.open(directoryPath + filename);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	name_ = "object/" + modelname;

	// メッシュ生成
	meshes_.emplace_back(new Mesh);
	Mesh* mesh = meshes_.back();
	int indexCountTex = 0;

	vector<Vector3> positions; // 頂点座標
	vector<Vector3> normals;   // 法線ベクトル
	vector<Vector2> texcoords; // テクスチャUV
	// 1行ずつ読み込む
	string line;
	while (getline(file, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//マテリアル
		if (key == "mtllib") {
			// マテリアルのファイル名読み込み
			string materialFilename;
			line_stream >> materialFilename;
			// マテリアル読み込み
			LoadMaterial(directoryPath, materialFilename);
		}
		// 先頭文字列がgならグループの開始
		if (key == "g") {

			// カレントメッシュの情報が揃っているなら
			if (mesh->GetName().size() > 0 && mesh->GetVertexCount() > 0) {
				// 頂点法線の平均によるエッジの平滑化
				if (smoothing) {
					mesh->CalculateSmoothedVertexNormals();
				}
				// 次のメッシュ生成
				meshes_.emplace_back(new Mesh);
				mesh = meshes_.back();
				indexCountTex = 0;
			}

			// グループ名読み込み
			string groupName;
			line_stream >> groupName;

			// メッシュに名前をセット
			mesh->SetName(groupName);
		}
		// 先頭文字列がvなら頂点座標
		if (key == "v") {
			// X,Y,Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			positions.emplace_back(position);
			VertexPos vertPos{};
			vertPos.pos = position;
			onlyTriangleVertices.emplace_back(vertPos);
		}
		// 先頭文字列がvtならテクスチャ
		if (key == "vt") {
			// U,V成分読み込み
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V方向反転
			texcoord.y = 1.0f - texcoord.y;
			// テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		// 先頭文字列がvnなら法線ベクトル
		if (key == "vn") {
			// X,Y,Z成分読み込み
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			// 法線ベクトルデータに追加
			normals.emplace_back(normal);
		}
		// 先頭文字列がusemtlならマテリアルを割り当てる
		if (key == "usemtl") {
			if (mesh->GetMaterial() == nullptr) {
				// マテリアルの名読み込み
				string materialName;
				line_stream >> materialName;

				// マテリアル名で検索し、マテリアルを割り当てる
				auto itr = materials_.find(materialName);
				if (itr != materials_.end()) {
					mesh->SetMaterial(itr->second);
				}
			}
		}
		// 先頭文字列がfならポリゴン（三角形）
		if (key == "f") {
			int faceIndexCount = 0;
			MyStruct::Meshes meshes;
			// 半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				// 頂点番号
				index_stream >> indexPosition;

				Material* material = mesh->GetMaterial();
				index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
				// マテリアル、テクスチャがある場合
				if (material && material->textureFilename_.size() > 0) {
					index_stream >> indexTexcoord;
					index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
					index_stream >> indexNormal;
					// 頂点データの追加
					Mesh::VertexPosNormalUv vertex{};
					vertex.pos = positions[indexPosition - 1];
					vertex.normal = normals[indexNormal - 1];
					vertex.uv = texcoords[indexTexcoord - 1];
					if (faceIndexCount <= 2) {
						meshes.meshPos[faceIndexCount] = MyMath::Vec3ToVec4(vertex.pos);
					}
					mesh->AddVertex(vertex);
					// エッジ平滑化用のデータを追加
					if (smoothing) {
						mesh->AddSmoothData(
							indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
					}
				}
				else {
					char c;
					index_stream >> c;
					// スラッシュ2連続の場合、頂点番号のみ
					if (c == '/') {
						// 頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = { 0, 0, 1 };
						vertex.uv = { 0, 0 };
						if (faceIndexCount <= 2) {
							meshes.meshPos[faceIndexCount] = MyMath::Vec3ToVec4(vertex.pos);
						}
						mesh->AddVertex(vertex);
					}
					else {
						index_stream.seekg(-1, ios_base::cur); // 1文字戻る
						index_stream >> indexTexcoord;
						index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
						index_stream >> indexNormal;
						// 頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = { 0, 0 };
						if (faceIndexCount <= 2) {
							meshes.meshPos[faceIndexCount] = MyMath::Vec3ToVec4(vertex.pos);
						}
						mesh->AddVertex(vertex);
						// エッジ平滑化用のデータを追加
						if (smoothing) {
							mesh->AddSmoothData(
								indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
						}
					}
				}
				// インデックスデータの追加
				if (faceIndexCount >= 3) {
					// 四角形ポリゴンの4点目なので、
					// 四角形の0,1,2,3の内 2,3,0で三角形を構築する
					mesh->AddIndex(static_cast<unsigned short>(indexCountTex - 1));
					mesh->AddIndex(static_cast<unsigned short>(indexCountTex));
					mesh->AddIndex(static_cast<unsigned short>(indexCountTex - 3));
				}
				else {
					mesh->AddIndex(static_cast<unsigned short>(indexCountTex));
				}
				indexCountTex++;
				faceIndexCount++;
			}
			mesheData.emplace_back(meshes);
		}
	}
	file.close();


	vertices = mesh->GetVertices();
	indices = mesh->GetIndices();

	// 頂点法線の平均によるエッジの平滑化
	if (smoothing) {
		mesh->CalculateSmoothedVertexNormals();
	}
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename) {
	// ファイルストリーム
	std::ifstream file;
	// マテリアルファイルを開く
	file.open(directoryPath + filename);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	Material* material = nullptr;

	// 1行ずつ読み込む
	string line;
	while (getline(file, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		// 先頭のタブ文字は無視する
		if (key[0] == '\t') {
			key.erase(key.begin()); // 先頭の文字を削除
		}

		// 先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl") {

			// 既にマテリアルがあれば
			if (material) {
				// マテリアルをコンテナに登録
				AddMaterial(material);
			}

			// 新しいマテリアルを生成
			material = Material::Create();
			// マテリアル名読み込み
			line_stream >> material->name_;
		}
		// 先頭文字列がKaならアンビエント色
		if (key == "Ka") {
			line_stream >> material->ambient_.x;
			line_stream >> material->ambient_.y;
			line_stream >> material->ambient_.z;
		}
		// 先頭文字列がKdならディフューズ色
		if (key == "Kd") {
			line_stream >> material->diffuse_.x;
			line_stream >> material->diffuse_.y;
			line_stream >> material->diffuse_.z;
		}
		// 先頭文字列がKsならスペキュラー色
		if (key == "Ks") {
			line_stream >> material->specular_.x;
			line_stream >> material->specular_.y;
			line_stream >> material->specular_.z;
		}
		// 先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd") {
			// テクスチャのファイル名読み込み
			line_stream >> material->textureFilename_;

			// フルパスからファイル名を取り出す
			size_t pos1;
			pos1 = material->textureFilename_.rfind('\\');
			if (pos1 != string::npos) {
				material->textureFilename_ = material->textureFilename_.substr(
					pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}

			pos1 = material->textureFilename_.rfind('/');
			if (pos1 != string::npos) {
				material->textureFilename_ = material->textureFilename_.substr(
					pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}
		}
	}
	// ファイルを閉じる
	file.close();


	if (material) {
		// マテリアルを登録
		AddMaterial(material);
	}
}

void Model::AddMaterial(Material* material) {
	// コンテナに登録
	materials_.emplace(material->name_, material);
}

void Model::LoadTextures() {
	int textureIndex = 0;
	string directoryPath = name_ + "/";

	for (auto& m : materials_) {
		Material* material = m.second;

		// テクスチャあり
		if (material->textureFilename_.size() > 0) {
			// マテリアルにテクスチャ読み込み
			material->LoadTexture(directoryPath);
			modelTextureHandle = material->GetTextureHadle();
			textureIndex++;
		}
		// テクスチャなし
		else {
			// マテリアルにテクスチャ読み込み
			material->LoadTexture("white1x1.png");
			textureIndex++;
		}
	}
}

void Model::Draw(
	const WorldTransform& worldTransform, const ViewProjection& viewProjection,const ViewProjection& lightViewProjection) {

	// ライトの描画
	LightData::GetInstance()->GetLightGroupData()->Draw(sCommandList_,6);

	// CBVをセット（ワールド行列）
	sCommandList_->SetGraphicsRootConstantBufferView(0, worldTransform.constBuff_->GetGPUVirtualAddress());

	// CBVをセット（ビュープロジェクション行列）
	sCommandList_->SetGraphicsRootConstantBufferView(1, viewProjection.constBuff_->GetGPUVirtualAddress());
	// CBVをセット（ビュープロジェクション行列）
	sCommandList_->SetGraphicsRootConstantBufferView(2,lightViewProjection.constBuff_->GetGPUVirtualAddress());


	for (int i = 0; i < meshes_.size(); i++) {
		// 全メッシュを描画
		meshes_[i]->Draw(sCommandList_, 3, 4, modelTextureHandle,5);
	}
}

void Model::ShadowDraw(const WorldTransform& worldTransform,const ViewProjection& viewProjection)
{
	// ライトの描画
	LightData::GetInstance()->GetLightGroupData()->Draw(sCommandList_,4);

	// CBVをセット（ワールド行列）
	sCommandList_->SetGraphicsRootConstantBufferView(0,worldTransform.constBuff_->GetGPUVirtualAddress());

	// CBVをセット（ビュープロジェクション行列）
	sCommandList_->SetGraphicsRootConstantBufferView(1,viewProjection.constBuff_->GetGPUVirtualAddress());

	for ( int i = 0; i < meshes_.size(); i++ )
	{
		// 全メッシュを描画
		meshes_[ i ]->Draw(sCommandList_,2,3,modelTextureHandle);
	}
}

std::string Model::GetName()
{
	return exportName_;
}





void Model::Draw(
	const WorldTransform& worldTransform,const ViewProjection& viewProjection,
	const uint32_t& textureHadle) {

	// ライトの描画
	LightData::GetInstance()->GetLightGroupData()->Draw(sCommandList_,4);

	// CBVをセット（ワールド行列）
	sCommandList_->SetGraphicsRootConstantBufferView(0,worldTransform.constBuff_->GetGPUVirtualAddress());

	// CBVをセット（ビュープロジェクション行列）
	sCommandList_->SetGraphicsRootConstantBufferView(1,viewProjection.constBuff_->GetGPUVirtualAddress());

	// 全メッシュを描画
	for ( auto& mesh : meshes_ )
	{
		mesh->Draw(
			sCommandList_,2,3,
			textureHadle);
	}
}