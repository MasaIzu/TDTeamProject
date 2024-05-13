#include "ParticleEditor.h"
#include "DirectXCore.h"
#include "Model.h"
#include <algorithm>
#include <cassert>
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>
#include "d3dx12.h"
#include <CreateResource.h>
#include <combaseapi.h>
#include "Numbers.h"
#pragma comment(lib, "d3dcompiler.lib")

#pragma warning(push)
#pragma warning(disable: 4820)
#include <d3d12.h>
#pragma warning(pop)

using namespace std;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* ParticleEditor::device = nullptr;
ID3D12GraphicsCommandList* ParticleEditor::cmdList = nullptr;
ComPtr<ID3D12RootSignature> ParticleEditor::rootsignature;
ComPtr<ID3D12RootSignature> ParticleEditor::rootSignature;//コンピュートシェーダー用
ComPtr<ID3D12PipelineState> ParticleEditor::pipelinestate;
ComPtr<ID3D12PipelineState> ParticleEditor::pipelineState;//コンピュートシェーダー用

std::unordered_map<std::string,ComPtr<ID3D12PipelineState>> ParticleEditor::m_pipelines;
ComPtr<ID3D12DescriptorHeap> ParticleEditor::m_cbvSrvUavHeap;

const std::string ParticleEditor::PSO_DEFAULT = "PSO_DEFAULT";
const std::string ParticleEditor::PSO_ADD = "PSO_ADD";
const std::string ParticleEditor::PSO_CS_INIT = "PSO_CS_INIT";
const std::string ParticleEditor::PSO_CS_EMIT = "PSO_CS_EMIT";
const std::string ParticleEditor::PSO_CS_UPDATE = "PSO_CS_UPDATE";
const std::string ParticleEditor::PSO_DRAW_PARTICLE = "PSO_DRAW_PARTICLE";
const std::string ParticleEditor::PSO_DRAW_PARTICLE_USE_TEX = "PSO_DRAW_PARTICLE_USE_TEX";

UINT ParticleEditor::m_incrementSize;

UINT ParticleEditor::m_cbvSrvUavDescriptorSize = 0;
uint32_t ParticleEditor::ParticleEdiCount = 0;

ParticleEditor::ShaderDetailCollision ParticleEditor::shaderDetailCollision_;

ComPtr<ID3D12Resource1> ParticleEditor::m_sceneDetailCollisionParameterCB;

template <class Archive>
void serialize(Archive& ar,ParticleEditor::SendPointGenerationParameters& sendParameters) {
	ar(cereal::make_nvp("StartPos",sendParameters.StartPos),cereal::make_nvp("EndPos",sendParameters.EndPos)
		,cereal::make_nvp("StartColor",sendParameters.StartColor),cereal::make_nvp("EndColor",sendParameters.EndColor)
		,cereal::make_nvp("Angle",sendParameters.Angle),cereal::make_nvp("Shot",sendParameters.Shot)
		,cereal::make_nvp("EndPointActive",sendParameters.EndPointActive),cereal::make_nvp("RandomVelocity",sendParameters.RandomVelocity)
		,cereal::make_nvp("RandomLife",sendParameters.RandomLife)
		,cereal::make_nvp("RandomSpeed",sendParameters.RandomSpeed),cereal::make_nvp("RandomScale",sendParameters.RandomScale)
		,cereal::make_nvp("Speed",sendParameters.Speed),cereal::make_nvp("LerpStrength",sendParameters.LerpStrength)
		,cereal::make_nvp("Scale",sendParameters.Scale)
		,cereal::make_nvp("ScaleTinker",sendParameters.ScaleTinker),cereal::make_nvp("MaxLife",sendParameters.MaxLife)
		,cereal::make_nvp("MaxParticleCount",sendParameters.MaxParticleCount),cereal::make_nvp("AdditiveSynthesis",sendParameters.AdditiveSynthesis)
		,cereal::make_nvp("RandomLifeMinMax",sendParameters.RandomLifeMinMax),cereal::make_nvp("RandomSpeedMinMax",sendParameters.RandomSpeedMinMax)
		,cereal::make_nvp("RandomScaleMinMax",sendParameters.RandomScaleMinMax),cereal::make_nvp("SpeedDivideSize",sendParameters.SpeedDivideSize)
		,cereal::make_nvp("ScaleDivideSize",sendParameters.ScaleDivideSize),cereal::make_nvp("GravityStrength",sendParameters.GravityStrength)
		,cereal::make_nvp("Interlocking",sendParameters.Interlocking),cereal::make_nvp("InterlockingStrength",sendParameters.InterlockingStrength)
		,cereal::make_nvp("InterlockingLength",sendParameters.InterlockingLength),cereal::make_nvp("ScaleDownLifeTime",sendParameters.ScaleDownLifeTime)
		,cereal::make_nvp("EmitParticles",sendParameters.EmitParticles),cereal::make_nvp("ParticleGroup",sendParameters.ParticleGroup)
		,cereal::make_nvp("ParticleGroupCount",sendParameters.ParticleGroupCount),cereal::make_nvp("GroupTimer",sendParameters.ExplosionTimer)
		,cereal::make_nvp("MaxGroupTimer",sendParameters.MaxExplosionTimer),cereal::make_nvp("RandomGroupTimerMinMax",sendParameters.RandomExplosionTimerMinMax)
		,cereal::make_nvp("RandomParticleExplosion",sendParameters.RandomParticleExplosion)
		,cereal::make_nvp("ShapeNumber",sendParameters.ShapeNumber),cereal::make_nvp("Width",sendParameters.Width)
		,cereal::make_nvp("Height",sendParameters.Height),cereal::make_nvp("Depth",sendParameters.Depth)
		,cereal::make_nvp("ShapeScale",sendParameters.ShapeScale),cereal::make_nvp("CollisionON",sendParameters.CollisionON)
		,cereal::make_nvp("GettingUpDownScale",sendParameters.GettingUpDownScale),cereal::make_nvp("VelocityAdjustment",sendParameters.VelocityAdjustment)
		,cereal::make_nvp("PostEffectPow",sendParameters.PostEffectPow)
	);
}

void ParticleEditor::StaticInitialize(ID3D12Device* Device)
{
	// nullptrチェック
	assert(Device);

	ParticleEditor::device = Device;

	m_cbvSrvUavDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// パイプライン初期化
	InitializeGraphicsPipeline();


	auto cbDetailCollisionDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(ShaderDetailCollision));
	m_sceneDetailCollisionParameterCB = MyFunction::CreateResource(cbDetailCollisionDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,D3D12_HEAP_TYPE_UPLOAD);
}

void ParticleEditor::StaticFinalize()
{

}

void ParticleEditor::PreDraw(ID3D12GraphicsCommandList* commandList,const bool& isAddition)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(ParticleEditor::cmdList == nullptr);

	// コマンドリストをセット
	ParticleEditor::cmdList = commandList;
	if ( !isAddition )
	{
		// パイプラインステートの設定
		commandList->SetPipelineState(m_pipelines[ PSO_DEFAULT ].Get());
	}
	else
	{
		// パイプラインステートの設定
		commandList->SetPipelineState(m_pipelines[ PSO_ADD ].Get());
	}

	// ルートシグネチャの設定
	commandList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void ParticleEditor::PostDraw()
{
	// コマンドリストを解除
	ParticleEditor::cmdList = nullptr;
}

uint32_t ParticleEditor::AddCollision(const Vector3& colPos,const Vector3& colScale)
{
	uint32_t count = shaderDetailCollision_.ColCount;
	if ( MaxColCount != shaderDetailCollision_.ColCount )
	{
		shaderDetailCollision_.Pos[ shaderDetailCollision_.ColCount ] = MyMath::Vec3ToVec4(colPos);
		shaderDetailCollision_.Scale[ shaderDetailCollision_.ColCount ] = MyMath::Vec3ToVec4(colScale);
		MyFunction::WriteToUploadHeapMemory(m_sceneDetailCollisionParameterCB.Get(),sizeof(ShaderDetailCollision),&shaderDetailCollision_);
		++shaderDetailCollision_.ColCount;
	}

	return count;
}

void ParticleEditor::ChangeCollision(const uint32_t& colcount,const Vector3& colPos,const Vector3& colScale) {
	shaderDetailCollision_.Pos[ colcount ] = MyMath::Vec3ToVec4(colPos);
	shaderDetailCollision_.Scale[ colcount ] = MyMath::Vec3ToVec4(colScale);
	MyFunction::WriteToUploadHeapMemory(m_sceneDetailCollisionParameterCB.Get(),sizeof(ShaderDetailCollision),&shaderDetailCollision_);
}

void ParticleEditor::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob; //ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/Particle/ParticleEditor/ParticleEditorVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob,&errorBlob);
	if ( FAILED(result) )
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/Particle/ParticleEditor/ParticleEditorGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob,&errorBlob);
	if ( FAILED(result) )
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}


	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/Particle/ParticleEditor/ParticleEditorPS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob,&errorBlob);
	if ( FAILED(result) )
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[ ] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",0,DXGI_FORMAT_R32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineAdd{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	//gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	gpipeline.RasterizerState.DepthClipEnable = true;//深度グリッピングを有効に
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	D3D12_RENDER_TARGET_BLEND_DESC blenddescAdd{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	blenddesc.BlendEnable = true;//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使う

	blenddescAdd = blenddesc;

	//加算合成
	blenddescAdd.BlendOp = D3D12_BLEND_OP_ADD;//加算
	blenddescAdd.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddescAdd.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う
	////減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う
	////色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//使わない
	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[ 0 ] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	gpipeline.NumRenderTargets = 2;	// 描画対象は2つ
	gpipeline.RTVFormats[ 0 ] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.RTVFormats[ 1 ] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	gpipelineAdd = gpipeline;
	gpipelineAdd.BlendState.RenderTarget[ 0 ] = blenddescAdd;

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1,0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[ 4 ] = {};
	rootparams[ 0 ].InitAsConstantBufferView(0);
	rootparams[ 1 ].InitAsConstantBufferView(1);
	rootparams[ 2 ].InitAsUnorderedAccessView(0);
	rootparams[ 3 ].InitAsDescriptorTable(1,&descRangeSRV,D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams),rootparams,1,&samplerDesc,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1_0,&rootSigBlob,&errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0,rootSigBlob->GetBufferPointer(),rootSigBlob->GetBufferSize(),IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootsignature.Get();
	gpipelineAdd.pRootSignature = rootsignature.Get();
	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline,IID_PPV_ARGS(&m_pipelines[ PSO_DEFAULT ]));
	assert(SUCCEEDED(result));

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipelineAdd,IID_PPV_ARGS(&m_pipelines[ PSO_ADD ]));
	assert(SUCCEEDED(result));

	{
		// カウンタ付き UAV はルートパラメータとして設定できない.
		CD3DX12_DESCRIPTOR_RANGE uavIndexList{};
		uavIndexList.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV,1,1);
		// ルートシグネチャの作成
		std::array<CD3DX12_ROOT_PARAMETER,5> rootParameters;
		rootParameters[ 0 ].InitAsConstantBufferView(0); // b0: Params
		rootParameters[ 1 ].InitAsConstantBufferView(1); // b1: Params
		rootParameters[ 2 ].InitAsConstantBufferView(2); // b2: Params
		rootParameters[ 3 ].InitAsUnorderedAccessView(0);// u0: Particles
		rootParameters[ 4 ].InitAsDescriptorTable(1,&uavIndexList); // u1: ParticleIndexList

		CD3DX12_ROOT_SIGNATURE_DESC uavRootSignatureDesc{};
		uavRootSignatureDesc.Init(
			UINT(rootParameters.size()),rootParameters.data(),
			1,&samplerDesc);

		ComPtr<ID3DBlob> signature,errBlob;
		D3D12SerializeRootSignature(&uavRootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0,&signature,&errBlob);
		device->CreateRootSignature(0,signature->GetBufferPointer(),signature->GetBufferSize(),IID_PPV_ARGS(&rootSignature));
	}

	ComPtr<ID3DBlob> csBlobInit; //コンピュートシェーダオブジェクト
	ComPtr<ID3DBlob> csBlobEmit;
	ComPtr<ID3DBlob> csBlobUpdate;
	// コンピュートシェーダーのコンパイル
	D3DCompileFromFile(
		L"Resources/Shaders/Particle/ParticleEditor/ParticleEditorCS.hlsl",
		nullptr,D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"initParticle","cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&csBlobInit,
		nullptr);
	D3DCompileFromFile(
		L"Resources/Shaders/Particle/ParticleEditor/ParticleEditorCS.hlsl",
		nullptr,D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"emitParticle","cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&csBlobEmit,
		nullptr);
	D3DCompileFromFile(
		L"Resources/Shaders/Particle/ParticleEditor/ParticleEditorCS.hlsl",
		nullptr,D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main","cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&csBlobUpdate,
		nullptr);

	// PSOの作成
	ComPtr<ID3D12PipelineState> CSpipelineState;
	D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = rootSignature.Get();

	//initialize用
	psoDesc.CS = CD3DX12_SHADER_BYTECODE(csBlobInit.Get());
	device->CreateComputePipelineState(&psoDesc,IID_PPV_ARGS(&CSpipelineState));
	m_pipelines[ PSO_CS_INIT ] = CSpipelineState;

	//emit用
	psoDesc.CS = CD3DX12_SHADER_BYTECODE(csBlobEmit.Get());
	device->CreateComputePipelineState(&psoDesc,IID_PPV_ARGS(&CSpipelineState));
	m_pipelines[ PSO_CS_EMIT ] = CSpipelineState;

	//update用
	psoDesc.CS = CD3DX12_SHADER_BYTECODE(csBlobUpdate.Get());
	device->CreateComputePipelineState(&psoDesc,IID_PPV_ARGS(&CSpipelineState));
	m_pipelines[ PSO_CS_UPDATE ] = CSpipelineState;

	const int MaxDescriptorCount = 2048; // SRV,CBV,UAV など.
	// SRV のディスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc{
	  D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	  MaxDescriptorCount,
	  D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
	  0
	};
	device->CreateDescriptorHeap(&cbvSrvUavHeapDesc,IID_PPV_ARGS(&m_cbvSrvUavHeap));

	m_incrementSize = device->GetDescriptorHandleIncrementSize(cbvSrvUavHeapDesc.Type);
}

void ParticleEditor::InitializeVerticeBuff()
{

	HRESULT result;

	UINT sizeVB = static_cast< UINT >( sizeof(GpuParticleElement) ) * particleCount;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProps.CreationNodeMask = 1;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProps.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProps.VisibleNodeMask = 1;


	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;


	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&heapProps,D3D12_HEAP_FLAG_NONE,&resourceDesc,D3D12_RESOURCE_STATE_COMMON,nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(GpuParticleElement);

	auto cbViewDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(ShaderViewParameters));
	m_sceneViewParameterCB = MyFunction::CreateResource(cbViewDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,D3D12_HEAP_TYPE_UPLOAD);

	auto cbDetailDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(ShaderDetailPointGenerationParameters));
	m_sceneDetailParameterCB = MyFunction::CreateResource(cbDetailDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,D3D12_HEAP_TYPE_UPLOAD);


	UINT64 bufferSize;
	bufferSize = sizeof(GpuParticleElement) * particleCount;
	auto resDescParticleElement = CD3DX12_RESOURCE_DESC::Buffer(
		bufferSize,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS
	);
	m_gpuParticleElement = MyFunction::CreateResource(resDescParticleElement,D3D12_RESOURCE_STATE_COMMON,nullptr,D3D12_HEAP_TYPE_DEFAULT);
	m_gpuParticleElement->SetName(L"ParticleElement");

	bufferSize = sizeof(UINT) * particleCount;
	UINT uavCounterAlign = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT - 1;
	bufferSize = UINT64(bufferSize + uavCounterAlign) & ~static_cast< UINT64 >( uavCounterAlign );
	bufferSize += sizeof(Vector4);   // カウンタをこの場所先頭に配置.

	auto resDescParticleIndexList = CD3DX12_RESOURCE_DESC::Buffer(
		bufferSize,D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	m_gpuParticleIndexList = MyFunction::CreateResource(resDescParticleIndexList,D3D12_RESOURCE_STATE_COMMON,nullptr,D3D12_HEAP_TYPE_DEFAULT);
	m_gpuParticleIndexList->SetName(L"ParticleIndexList");

	UINT64 offsetToCounter = bufferSize - sizeof(Vector4);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.NumElements = particleCount;
	// インデックス用バッファの後方でカウンタを設置する.
	uavDesc.Buffer.CounterOffsetInBytes = offsetToCounter;
	uavDesc.Buffer.StructureByteStride = sizeof(UINT);


	m_handleCpu = m_cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart();
	m_handleGpu = m_cbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart();

	processedCommandsHandle_ = m_handleCpu;
	device->CreateUnorderedAccessView(
		m_gpuParticleIndexList.Get(),
		m_gpuParticleIndexList.Get(),
		&uavDesc,processedCommandsHandle_
	);

}

void ParticleEditor::SetTextureHandle(uint32_t textureHandle) {
	textureHandle_ = textureHandle;
}

Vector4 ParticleEditor::GetFirstColorParticle() {
	return shaderDetailParameters.StartColor;
}

void ParticleEditor::Initialize(const uint32_t& ParticleCount,const bool& isEditUpdate_,const std::string& FileName)
{
	particleCount = ParticleCount;
	sendParameters.MaxParticleCount = particleCount;
	InitializeVerticeBuff();

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	device->CreateDescriptorHeap(&heapDesc,IID_PPV_ARGS(&descriptorHeap));

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = particleCount;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.StructureByteStride = sizeof(GpuParticleElement);

	device->CreateUnorderedAccessView(
		vertBuff.Get(),
		nullptr,
		&uavDesc,
		descriptorHeap->GetCPUDescriptorHandleForHeapStart()
	);


	if ( FileName != "Nothing" )
	{
		isLoadFile = true;

		KeepFileName = FileName;

		std::string FullPath = FilePath + FileName + ".json";
		SendPointGenerationParameters ReadParameters;
		std::ifstream is(FullPath);
		cereal::JSONInputArchive archive(is);
		archive(cereal::make_nvp(FullPath,ReadParameters));

		LoadFileParameter(ReadParameters);
		SetParameter();

		EditUpdate();
	}
	isEditUpdate = isEditUpdate_;

	particleEdiCount = ParticleEdiCount;
	++ParticleEdiCount;
}

void ParticleEditor::SetAddShield(AddShield* AddShield)
{
	addShield = AddShield;
}

void ParticleEditor::Initialize(const std::string& FileName)
{

	std::string FullPath = FilePath + FileName + ".json";
	SendPointGenerationParameters ReadParameters;
	std::ifstream is(FullPath);
	cereal::JSONInputArchive archive(is);
	archive(cereal::make_nvp(FullPath,ReadParameters));

	LoadFileParameter(ReadParameters);
	SetParameter();

	EditUpdate();

	particleCount = ReadParameters.MaxParticleCount;
	sendParameters.MaxParticleCount = particleCount;
	InitializeVerticeBuff();

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	device->CreateDescriptorHeap(&heapDesc,IID_PPV_ARGS(&descriptorHeap));

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = particleCount;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.StructureByteStride = sizeof(GpuParticleElement);

	device->CreateUnorderedAccessView(
		vertBuff.Get(),
		nullptr,
		&uavDesc,
		descriptorHeap->GetCPUDescriptorHandleForHeapStart()
	);

}

void ParticleEditor::EditUpdate()
{

#ifdef _Editor

	if ( isParticleActiveCheckBox )
	{
		isParticleActiveCheckBox = false;
		sendParameters.Shot = false;
	}

	string PrticleCounter = to_string(particleEdiCount);
	PrticleCounter = "ParticleEditor Count" + PrticleCounter;

	if ( isEditUpdate )
	{
		sendParameters.isLoad = false;
		ImGui::Begin(PrticleCounter.c_str());

		// ファイル名の入力フィールド
		static char fileName[ 128 ] = "";
		if ( !isDeletFileFirstTime )
		{

			ImGui::Checkbox("StageDraw",&isStageDraw);
			ImGui::Checkbox("AdditiveSynthesis",&sendParameters.AdditiveSynthesis);

			ImGui::SetNextItemOpen(true,ImGuiCond_Once);
			if ( ImGui::TreeNode("File Tree") )
			{

				ImGui::InputText("new file name",fileName,IM_ARRAYSIZE(fileName));
				isCreateNewFile = ImGui::Button("create a new file");

				std::vector<std::string> fileList = GetFileList(FilePath);
				if ( ImGui::BeginListBox("##filelist") )
				{
					for ( int i = 0; i < fileList.size(); i++ )
					{
						const bool isSelected = ( selectedIndex == i );
						if ( ImGui::Selectable(fileList[ i ].c_str(),isSelected) )
						{
							selectedIndex = i;
							SelectedFileName = fileList[ i ];  // 選択されたファイル名を保存
						}

						// 選択された項目を表示領域内にスクロール
						if ( isSelected )
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndListBox();
				}

				isPushSave = ImGui::Button("save");
				ImGui::SameLine();
				isPushLoad = ImGui::Button("load");
				ImGui::SameLine();
				isPushReset = ImGui::Button("reset");

				// 削除ボタンの前にスタイルをプッシュ
				ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(1.0f,0.0f,0.0f,1.0f));  // 赤色
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(1.0f,0.3f,0.3f,1.0f));  // ホバー時の色
				ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4(0.8f,0.0f,0.0f,1.0f));  // アクティブ時の色

				// 削除ボタン
				isDeletFileFirstTime = ImGui::Button("delete file");

				// スタイルをポップ
				ImGui::PopStyleColor(3);

				ImGui::TreePop();
			}

			ImGui::Text("");

			ImGui::SetNextItemOpen(true,ImGuiCond_Once);
			if ( ImGui::TreeNode("Shape Type") )
			{
				// リストボックスを表示
				ImGui::ListBox("Type",&selectedItem,shapes,IM_ARRAYSIZE(shapes));

				if ( ImGui::Button("Select Shape") )
				{
					sendParameters.ShapeNumber = selectedItem;
				}

				if ( sendParameters.ShapeNumber == 0 )
				{

				}
				else if ( sendParameters.ShapeNumber == 1 )
				{
					ImGui::SliderFloat("Width",&sendParameters.Width,0.0f,100.0f);
					ImGui::SliderFloat("Height",&sendParameters.Height,0.0f,100.0f);
					ImGui::SliderFloat("Depth",&sendParameters.Depth,0.0f,100.0f);
					ImGui::SliderFloat("ShapeScaleX",&sendParameters.ShapeScale[ 0 ],0.0f,100.0f);
					ImGui::SliderFloat("ShapeScaleY",&sendParameters.ShapeScale[ 1 ],0.0f,100.0f);
					ImGui::SliderFloat("ShapeScaleZ",&sendParameters.ShapeScale[ 2 ],0.0f,100.0f);
				}
				else if ( sendParameters.ShapeNumber == 2 )
				{
					ImGui::SliderFloat("ShapeScaleX",&sendParameters.ShapeScale[ 0 ],0.0f,100.0f);
					ImGui::SliderFloat("ShapeScaleY",&sendParameters.ShapeScale[ 1 ],0.0f,100.0f);
					ImGui::SliderFloat("ShapeScaleZ",&sendParameters.ShapeScale[ 2 ],0.0f,100.0f);
				}
				else if ( sendParameters.ShapeNumber == 3 )
				{
					ImGui::Text("Making");
				}

				ImGui::TreePop();
			}

			ImGui::Text("");

			if ( ImGui::TreeNode("Flag") )
			{
				ImGui::Checkbox("ParticleActive",&sendParameters.Shot);
				if ( ImGui::Button("OneTimeParticleActive") )
				{
					sendParameters.Shot = true;
					isParticleActiveCheckBox = true;
				}
				ImGui::Checkbox("EmitParticles",&sendParameters.EmitParticles);
				ImGui::Checkbox("RandomVelocity",&sendParameters.RandomVelocity);
				if ( sendParameters.RandomVelocity )
				{
					ImGui::SliderFloat3("VelocityAdjustment",sendParameters.VelocityAdjustment,1.0f,30.0f);
				}
				ImGui::Checkbox("ScaleDownLifeTime",&sendParameters.ScaleDownLifeTime);
				ImGui::Checkbox("CollisionON",&sendParameters.CollisionON);
				ImGui::Checkbox("GettingUpDownScale",&sendParameters.GettingUpDownScale);
				ImGui::Checkbox("EndPointActive",&sendParameters.EndPointActive);
				if ( sendParameters.EndPointActive )
				{
					ImGui::SliderFloat3("EndPointPos",sendParameters.EndPos,-300.0f,300.0f);
					ImGui::SliderFloat("LerpStrength",&sendParameters.LerpStrength,0.01f,1.0f);
				}


				ImGui::TreePop();
			}

			//ImGui::SetNextTreeNodeOpen(true,ImGuiSetCond_Once);
			if ( ImGui::TreeNode("Normal Parameters") )
			{
				ImGui::SliderFloat("PostEffectPow",&sendParameters.PostEffectPow,0.0f,1.0f);
				ImGui::SliderFloat3("StartPos",sendParameters.StartPos,-300.0f,300.0f);
				ImGui::ColorEdit4("StartColor",sendParameters.StartColor,ImGuiColorEditFlags_Float);
				ImGui::SliderFloat("StartColorAlpha",&sendParameters.StartColor[ 3 ],0.0f,1.0f);
				ImGui::ColorEdit4("EndColor",sendParameters.EndColor,ImGuiColorEditFlags_Float);
				ImGui::SliderFloat("EndColorAlpha",&sendParameters.EndColor[ 3 ],0.0f,1.0f);
				ImGui::SliderFloat("Speed",&sendParameters.Speed,0.0f,30.0f);
				ImGui::SliderFloat("LifeTime",&sendParameters.MaxLife,1.0f,500.0f);
				ImGui::SliderFloat("Scale",&sendParameters.Scale,0.01f,30.0f);
				ImGui::SliderFloat("ScaleTinker",&sendParameters.ScaleTinker,-1.0f,1.0f);
				ImGui::SliderFloat("AngleX",&sendParameters.Angle[ 0 ],0.0f,360.0f);
				ImGui::SliderFloat("AngleY",&sendParameters.Angle[ 1 ],0.0f,360.0f);
				ImGui::SliderFloat("AngleZ",&sendParameters.Angle[ 2 ],0.0f,360.0f);

				ImGui::TreePop();
			}

			if ( ImGui::TreeNode("Random Parameters") )
			{
				ImGui::Checkbox("RandomLife",&sendParameters.RandomLife);
				if ( sendParameters.RandomLife )
				{
					ImGui::SliderFloat("LifeMin",&sendParameters.RandomLifeMinMax[ 0 ],1,500);
					ImGui::SliderFloat("LifeMax",&sendParameters.RandomLifeMinMax[ 1 ],2,500);
					if ( sendParameters.RandomLifeMinMax[ 0 ] >= sendParameters.RandomLifeMinMax[ 1 ] )
					{
						sendParameters.RandomLifeMinMax[ 0 ] = sendParameters.RandomLifeMinMax[ 1 ] - 1.0f;
					}
				}
				ImGui::Checkbox("RandomSpeed",&sendParameters.RandomSpeed);
				if ( sendParameters.RandomSpeed )
				{
					ImGui::SliderFloat("SpeedMin",&sendParameters.RandomSpeedMinMax[ 0 ],0,10);
					ImGui::SliderFloat("SpeedMax",&sendParameters.RandomSpeedMinMax[ 1 ],1.1f,10);
					if ( sendParameters.RandomSpeedMinMax[ 0 ] >= sendParameters.RandomSpeedMinMax[ 1 ] )
					{
						sendParameters.RandomSpeedMinMax[ 0 ] = sendParameters.RandomSpeedMinMax[ 1 ] - 1.0f;
					}
					ImGui::SliderFloat("SpeedDivideSize",&sendParameters.SpeedDivideSize,1,100);
				}
				ImGui::Checkbox("RandomScale",&sendParameters.RandomScale);
				if ( sendParameters.RandomScale )
				{
					ImGui::SliderFloat("ScaleMin",&sendParameters.RandomScaleMinMax[ 0 ],1,100);
					ImGui::SliderFloat("ScaleMax",&sendParameters.RandomScaleMinMax[ 1 ],2,100);
					if ( sendParameters.RandomScaleMinMax[ 0 ] >= sendParameters.RandomScaleMinMax[ 1 ] )
					{
						sendParameters.RandomScaleMinMax[ 0 ] = sendParameters.RandomScaleMinMax[ 1 ] - 1.0f;
					}
					ImGui::SliderFloat("ScaleDivideSize",&sendParameters.ScaleDivideSize,1,100);
				}
				ImGui::Checkbox("GravityActive",&isGravityStrengthActive);
				if ( isGravityStrengthActive )
				{
					ImGui::SliderFloat("GravityStrength",&GravityStrength,-1,1);
					ImGui::SliderFloat("GravityStrengthDiv",&GravityStrengthDiv,1,100);

					sendParameters.GravityStrength = GravityStrength / GravityStrengthDiv;
				}
				else
				{
					sendParameters.GravityStrength = 0;
				}

				ImGui::Checkbox("Interlocking",&sendParameters.Interlocking);
				if ( sendParameters.Interlocking )
				{
					ImGui::SliderFloat("InterlockingStrength",&sendParameters.InterlockingStrength,0,1);
					ImGui::SliderFloat("InterlockingLength",&sendParameters.InterlockingLength,1,100);
				}

				ImGui::Checkbox("ParticleGroup",&sendParameters.ParticleGroup);
				if ( sendParameters.ParticleGroup )
				{
					ImGui::SliderInt("ParticleGroupCount",&sendParameters.ParticleGroupCount,1,1000);
					ImGui::SliderFloat("GroupTimer",&sendParameters.ExplosionTimer,0,300);
					ImGui::Checkbox("RandomParticleExplosion",&sendParameters.RandomParticleExplosion);
					if ( sendParameters.RandomParticleExplosion )
					{
						ImGui::SliderFloat("RandomGroupTimerMin",&sendParameters.RandomExplosionTimerMinMax[ 0 ],1,300);
						ImGui::SliderFloat("RandomGroupTimerMax",&sendParameters.RandomExplosionTimerMinMax[ 1 ],2,300);
						if ( sendParameters.RandomExplosionTimerMinMax[ 0 ] >= sendParameters.RandomExplosionTimerMinMax[ 1 ] )
						{
							sendParameters.RandomExplosionTimerMinMax[ 0 ] = sendParameters.RandomExplosionTimerMinMax[ 1 ] - 1.0f;
						}
					}
					ImGui::SliderFloat("MaxGroupTimer",&sendParameters.MaxExplosionTimer,sendParameters.ExplosionTimer,2000);
				}

				ImGui::TreePop();
			}
			ImGui::Text("");

			if ( ImGui::Button("MakeCollision") )
			{
				isMakeCol = true;
				addShield->Initialize();
			}
			if ( isMakeCol )
			{
				ImGui::SliderFloat3("ColPos",ColPos,-500,500);
				ImGui::SliderFloat3("ColScale",ColScale,0,100);
				Vector3 colPos;
				colPos = ColPos;
				Vector3 colScale;
				colScale = ColScale;
				addShield->Update(colPos,colScale);

				if ( ImGui::Button("SetCol") )
				{
					addShield->SetFin();
					isMakeCol = false;
					for ( int i = 0; i < 3; i++ )
					{
						ColPos[ i ] = 0;
						ColScale[ i ] = 1;
					}
				}
			}


			ImGui::Text("");
			int ParticleCount = static_cast< int >( sendParameters.MaxParticleCount );
			ImGui::Text("MaxParticle : %d",particleCount);
			ImGui::Text("NowParticleCount : %d",ParticleCount);
			ImGui::InputInt("NewParticlePieces",&NewParticleCount);
			isSetNewParticleCount = ImGui::Button("set new pieces");
		}
		else
		{
			ImGui::Text("delete the file?");
			if ( ImGui::Button("YES") )
			{
				isDeletFileSecondTime = true;
			}
			ImGui::SameLine();
			if ( ImGui::Button(" NO ") )
			{
				isDeletFileFirstTime = false;
			}
		}

		if ( ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) )
		{
			isWindowFocus = true;
		}
		else
		{
			isWindowFocus = false;
		}

		ImGui::End();

		std::string FullPath = FilePath + SelectedFileName;

		if ( isSetNewParticleCount )
		{
			if ( particleCount < static_cast< uint32_t >( NewParticleCount ) )
			{
				NewParticleCount = particleCount;
			}
			sendParameters.MaxParticleCount = NewParticleCount;
			sendParameters.Shot = false;
		}

		SetParameter();
		MyFunction::WriteToUploadHeapMemory(m_sceneDetailParameterCB.Get(),sizeof(ShaderDetailPointGenerationParameters),&shaderDetailParameters);


		if ( isPushSave )
		{
			std::ofstream os(FullPath);
			cereal::JSONOutputArchive archive(os);
			archive(cereal::make_nvp(FullPath,sendParameters));
		}

		if ( isPushLoad )
		{
			SendPointGenerationParameters ReadParameters;
			std::ifstream is(FullPath);
			cereal::JSONInputArchive archive(is);
			archive(cereal::make_nvp(FullPath,ReadParameters));

			LoadFileParameter(ReadParameters);
		}

		if ( isPushReset )
		{
			SendPointGenerationParameters ReadParameters;
			LoadFileParameter(ReadParameters);
		}

		if ( isCreateNewFile )
		{
			NewFileName = "Resources/ParticleData/";
			NewFileName += fileName;
			NewFileName += ".json";

			std::ofstream os(NewFileName);
			cereal::JSONOutputArchive archive(os);
			archive(cereal::make_nvp(NewFileName,sendParameters));
		}

		if ( selectedIndex != -1 && isDeletFileFirstTime && isDeletFileSecondTime )
		{
			std::filesystem::path filePath = FullPath;
			std::filesystem::remove(filePath);  // ファイルを削除
			selectedIndex = -1;  // 選択をリセット
			isDeletFileFirstTime = false;
			isDeletFileSecondTime = false;
		}
	}

#endif
}

void ParticleEditor::Draw(const ViewProjection& view)
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = DirectXCore::GetInstance()->GetCommandList();
	PreDraw(commandList,sendParameters.AdditiveSynthesis);

	Matrix4 constMatToSend = view.matView;
	constMatToSend *= view.matProjection;

	shaderViewParameters.mat = constMatToSend;
	shaderViewParameters.matBillboard = view.matBillboard;
	MyFunction::WriteToUploadHeapMemory(m_sceneViewParameterCB.Get(),sizeof(ShaderViewParameters),&shaderViewParameters);

	// nullptrチェック
	assert(device);
	assert(ParticleEditor::cmdList);


	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0,1,&vbView);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0,m_sceneViewParameterCB->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1,m_sceneDetailParameterCB->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootUnorderedAccessView(2,m_gpuParticleElement->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(cmdList,3,textureHandle_);
	// 描画コマンド
	cmdList->DrawInstanced(static_cast< UINT >( particleCount ),1,0,0);

	PostDraw();

}

void ParticleEditor::CSUpdate(ID3D12GraphicsCommandList* commandList)
{
	CSCmd(commandList);
}

void ParticleEditor::CSUpdate(ID3D12GraphicsCommandList* commandList,const Vector4& StartPos,const uint32_t& isParticleActive)
{
	shaderDetailParameters.StartPos = StartPos;
	shaderDetailParameters.Shot = isParticleActive;
	if ( shaderDetailParameters.EndPointActive )
	{
		Vector4 EndPos;
		EndPos = sendParameters.EndPos;
		shaderDetailParameters.EndPos = StartPos + EndPos;
	}
	CSCmd(commandList);
}

void ParticleEditor::CSUpdate(ID3D12GraphicsCommandList* commandList,const uint32_t& isParticleActive,const Vector4& EndPos)
{
	shaderDetailParameters.EndPos = EndPos;
	shaderDetailParameters.Shot = isParticleActive;
	shaderDetailParameters.EndPointActive = isParticleActive;
	CSCmd(commandList);
}

void ParticleEditor::CSUpdate(ID3D12GraphicsCommandList* commandList,const uint32_t& isParticleActive,const uint32_t& isEndPointActive,const Vector4& EndPos)
{
	shaderDetailParameters.EndPos = EndPos;
	shaderDetailParameters.Shot = isParticleActive;
	shaderDetailParameters.EndPointActive = isEndPointActive;
	CSCmd(commandList);
}

void ParticleEditor::CSUpdate(ID3D12GraphicsCommandList* commandList,const uint32_t& isParticleActive,const uint32_t& isEndPointActive,const Vector4& EndPos,const float& speed)
{
	shaderDetailParameters.EndPos = EndPos;
	shaderDetailParameters.Shot = isParticleActive;
	shaderDetailParameters.EndPointActive = isEndPointActive;
	if ( isEndPointActive )
	{
		shaderDetailParameters.RandomSpeed = false;
	}
	else
	{
		shaderDetailParameters.RandomSpeed = true;
	}
	shaderDetailParameters.Speed = speed;
	CSCmd(commandList);
}

void ParticleEditor::CSUpdate(ID3D12GraphicsCommandList* commandList,const Vector4& StartPos,const Vector4& EndPos)
{
	shaderDetailParameters.StartPos = StartPos;
	shaderDetailParameters.EndPos = EndPos;
	CSCmd(commandList);
}

void ParticleEditor::CSUpdate(ID3D12GraphicsCommandList* commandList,const Vector4& StartPos,const Vector4& EndPos,const uint32_t& isParticleActive)
{
	shaderDetailParameters.StartPos = StartPos;
	shaderDetailParameters.EndPos = EndPos;
	shaderDetailParameters.Shot = isParticleActive;
	CSCmd(commandList);
}

void ParticleEditor::CSCmd(ID3D12GraphicsCommandList* commandList)
{
	ID3D12DescriptorHeap* ppHeaps[ ] = { m_cbvSrvUavHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps),ppHeaps);
	//初期化
	if ( m_frameCount == 0 )
	{
		if ( !isLoadFile )
		{
			shaderDetailParameters.MaxParticleCount = particleCount;
		}

		MyFunction::WriteToUploadHeapMemory(m_sceneDetailParameterCB.Get(),sizeof(ShaderDetailPointGenerationParameters),&shaderDetailParameters);

		CD3DX12_RESOURCE_BARRIER transitionBarrier[ 2 ];
		transitionBarrier[ 0 ] = CD3DX12_RESOURCE_BARRIER::Transition(m_gpuParticleElement.Get(),D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		transitionBarrier[ 1 ] = CD3DX12_RESOURCE_BARRIER::Transition(m_gpuParticleIndexList.Get(),D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		commandList->ResourceBarrier(2,transitionBarrier);

		D3D12_GPU_DESCRIPTOR_HANDLE cbvSrvUavHandle = m_cbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart();
		// Particle の初期化コード.
		commandList->SetComputeRootSignature(rootSignature.Get());

		commandList->SetComputeRootConstantBufferView(0,m_sceneViewParameterCB->GetGPUVirtualAddress());
		commandList->SetComputeRootConstantBufferView(1,m_sceneDetailParameterCB->GetGPUVirtualAddress());
		commandList->SetComputeRootConstantBufferView(2,m_sceneDetailCollisionParameterCB->GetGPUVirtualAddress());
		commandList->SetComputeRootUnorderedAccessView(3,m_gpuParticleElement->GetGPUVirtualAddress());
		commandList->SetComputeRootDescriptorTable(4,m_handleGpu);
		commandList->SetPipelineState(m_pipelines[ PSO_CS_INIT ].Get());

		UINT invokeCount = particleCount / 128 + 1;
		commandList->Dispatch(invokeCount,1,1);
	}

	MyFunction::WriteToUploadHeapMemory(m_sceneDetailParameterCB.Get(),sizeof(ShaderDetailPointGenerationParameters),&shaderDetailParameters);
	{
		// Particle の発生.
		D3D12_GPU_DESCRIPTOR_HANDLE cbvSrvUavHandle = m_cbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart();

		commandList->SetComputeRootSignature(rootSignature.Get());
		commandList->SetComputeRootConstantBufferView(0,m_sceneViewParameterCB->GetGPUVirtualAddress());
		commandList->SetComputeRootConstantBufferView(1,m_sceneDetailParameterCB->GetGPUVirtualAddress());
		commandList->SetComputeRootConstantBufferView(2,m_sceneDetailCollisionParameterCB->GetGPUVirtualAddress());
		commandList->SetComputeRootUnorderedAccessView(3,m_gpuParticleElement->GetGPUVirtualAddress());
		commandList->SetComputeRootDescriptorTable(4,m_handleGpu);
		commandList->SetPipelineState(m_pipelines[ PSO_CS_EMIT ].Get());

		UINT invokeCount = particleCount / 128 + 1;
		{
			commandList->Dispatch(invokeCount,1,1);
		}

		CD3DX12_RESOURCE_BARRIER barriers[ ] = {
		  CD3DX12_RESOURCE_BARRIER::UAV(m_gpuParticleElement.Get()),
		  CD3DX12_RESOURCE_BARRIER::UAV(m_gpuParticleIndexList.Get()),
		};
		commandList->ResourceBarrier(_countof(barriers),barriers);

		// Particle の更新処理.
		commandList->SetPipelineState(m_pipelines[ PSO_CS_UPDATE ].Get());
		commandList->Dispatch(invokeCount,1,1);
		commandList->ResourceBarrier(_countof(barriers),barriers);
	}

	++m_frameCount;
}

// 指定ディレクトリのファイルリストを取得
std::vector<std::string> ParticleEditor::GetFileList(const std::filesystem::path& directory) {
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

void ParticleEditor::SetParameter()
{
	shaderDetailParameters.StartPos = sendParameters.StartPos;
	shaderDetailParameters.EndPos = sendParameters.EndPos;
	shaderDetailParameters.EndColor = sendParameters.EndColor;
	shaderDetailParameters.StartColor = sendParameters.StartColor;
	shaderDetailParameters.Angle = sendParameters.Angle;
	shaderDetailParameters.Shot = sendParameters.Shot;
	shaderDetailParameters.EndPointActive = sendParameters.EndPointActive;
	shaderDetailParameters.RandomVelocity = sendParameters.RandomVelocity;
	shaderDetailParameters.RandomLife = sendParameters.RandomLife;
	shaderDetailParameters.RandomSpeed = sendParameters.RandomSpeed;
	shaderDetailParameters.RandomScale = sendParameters.RandomScale;
	shaderDetailParameters.Speed = sendParameters.Speed;
	shaderDetailParameters.LerpStrength = sendParameters.LerpStrength;
	shaderDetailParameters.Scale = sendParameters.Scale;
	shaderDetailParameters.ScaleTinker = sendParameters.ScaleTinker;
	shaderDetailParameters.MaxLife = sendParameters.MaxLife;
	shaderDetailParameters.MaxParticleCount = sendParameters.MaxParticleCount;
	shaderDetailParameters.AdditiveSynthesis = sendParameters.AdditiveSynthesis;
	shaderDetailParameters.RandomLifeMinMax = sendParameters.RandomLifeMinMax;
	shaderDetailParameters.RandomSpeedMinMax = sendParameters.RandomSpeedMinMax;
	shaderDetailParameters.RandomScaleMinMax = sendParameters.RandomScaleMinMax;
	shaderDetailParameters.SpeedDivideSize = sendParameters.SpeedDivideSize;
	shaderDetailParameters.ScaleDivideSize = sendParameters.ScaleDivideSize;
	shaderDetailParameters.GravityStrength = sendParameters.GravityStrength;
	shaderDetailParameters.Interlocking = sendParameters.Interlocking;
	shaderDetailParameters.InterlockingStrength = sendParameters.InterlockingStrength;
	shaderDetailParameters.InterlockingLength = sendParameters.InterlockingLength;
	shaderDetailParameters.ScaleDownLifeTime = sendParameters.ScaleDownLifeTime;
	shaderDetailParameters.EmitParticles = sendParameters.EmitParticles;

	shaderDetailParameters.ParticleGroup = sendParameters.ParticleGroup;
	shaderDetailParameters.ParticleGroupCount = sendParameters.ParticleGroupCount;
	shaderDetailParameters.ExplosionTimer = sendParameters.ExplosionTimer;
	shaderDetailParameters.MaxExplosionTimer = sendParameters.MaxExplosionTimer;
	shaderDetailParameters.RandomExplosionTimerMinMax = sendParameters.RandomExplosionTimerMinMax;
	shaderDetailParameters.RandomParticleExplosion = sendParameters.RandomParticleExplosion;
	shaderDetailParameters.ShapeNumber = sendParameters.ShapeNumber;
	shaderDetailParameters.Width = sendParameters.Width;
	shaderDetailParameters.Height = sendParameters.Height;
	shaderDetailParameters.Depth = sendParameters.Depth;
	shaderDetailParameters.ShapeScale = sendParameters.ShapeScale;
	shaderDetailParameters.CollisionON = sendParameters.CollisionON;
	shaderDetailParameters.GettingUpDownScale = sendParameters.GettingUpDownScale;
	shaderDetailParameters.VelocityAdjustment = sendParameters.VelocityAdjustment;
	shaderDetailParameters.PostEffectPow = sendParameters.PostEffectPow;

	shaderDetailParameters.isLoad = sendParameters.isLoad;
}

void ParticleEditor::LoadFileParameter(const SendPointGenerationParameters& params)
{
	//配列コピー操作
	memcpy(sendParameters.StartPos,params.StartPos,sizeof(params.StartPos));
	memcpy(sendParameters.EndPos,params.EndPos,sizeof(params.EndPos));
	memcpy(sendParameters.EndColor,params.EndColor,sizeof(params.EndColor));
	memcpy(sendParameters.StartColor,params.StartColor,sizeof(params.StartColor));
	memcpy(sendParameters.Angle,params.Angle,sizeof(params.Angle));
	memcpy(sendParameters.RandomLifeMinMax,params.RandomLifeMinMax,sizeof(params.RandomLifeMinMax));
	memcpy(sendParameters.RandomSpeedMinMax,params.RandomSpeedMinMax,sizeof(params.RandomSpeedMinMax));
	memcpy(sendParameters.RandomScaleMinMax,params.RandomScaleMinMax,sizeof(params.RandomScaleMinMax));
	memcpy(sendParameters.RandomExplosionTimerMinMax,params.RandomExplosionTimerMinMax,sizeof(params.RandomExplosionTimerMinMax));
	memcpy(sendParameters.ShapeScale,params.ShapeScale,sizeof(params.ShapeScale));
	memcpy(sendParameters.VelocityAdjustment,params.VelocityAdjustment,sizeof(params.VelocityAdjustment));

	sendParameters.Shot = params.Shot;
	sendParameters.EndPointActive = params.EndPointActive;
	sendParameters.RandomVelocity = params.RandomVelocity;
	sendParameters.RandomLife = params.RandomLife;
	sendParameters.RandomSpeed = params.RandomSpeed;
	sendParameters.RandomScale = params.RandomScale;
	sendParameters.Speed = params.Speed;
	sendParameters.LerpStrength = params.LerpStrength;
	sendParameters.Scale = params.Scale;
	sendParameters.ScaleTinker = params.ScaleTinker;
	sendParameters.MaxLife = params.MaxLife;
	sendParameters.MaxParticleCount = params.MaxParticleCount;
	sendParameters.AdditiveSynthesis = params.AdditiveSynthesis;

	sendParameters.SpeedDivideSize = params.SpeedDivideSize;
	sendParameters.ScaleDivideSize = params.ScaleDivideSize;
	sendParameters.GravityStrength = params.GravityStrength;

	if ( sendParameters.GravityStrength != 0 )
	{
		isGravityStrengthActive = true;
	}

	sendParameters.Interlocking = params.Interlocking;
	sendParameters.InterlockingStrength = params.InterlockingStrength;
	sendParameters.InterlockingLength = params.InterlockingLength;
	sendParameters.ScaleDownLifeTime = params.ScaleDownLifeTime;
	sendParameters.EmitParticles = params.EmitParticles;

	sendParameters.ParticleGroup = params.ParticleGroup;
	sendParameters.ParticleGroupCount = params.ParticleGroupCount;
	sendParameters.ExplosionTimer = params.ExplosionTimer;
	sendParameters.MaxExplosionTimer = params.MaxExplosionTimer;
	sendParameters.RandomParticleExplosion = params.RandomParticleExplosion;
	sendParameters.ShapeNumber = params.ShapeNumber;
	sendParameters.Width = params.Width;
	sendParameters.Height = params.Height;
	sendParameters.Depth = params.Depth;
	sendParameters.CollisionON = params.CollisionON;
	sendParameters.GettingUpDownScale = params.GettingUpDownScale;
	sendParameters.PostEffectPow = params.PostEffectPow;

	sendParameters.isLoad = true;
}

bool ParticleEditor::GetIsWindowFocus()
{
	return isWindowFocus;
}

bool ParticleEditor::IsStageDraw()
{
	return isStageDraw;
}

void ParticleEditor::CopyData()
{

	VertexPos* outPutDeta = nullptr;
	vertBuff->Map(0,nullptr,( void** ) &outPutDeta);
	memcpy(Particles.data(),outPutDeta,Particles.size() * sizeof(VertexPos));
	vertBuff->Unmap(0,nullptr);

}

