#include "Trail.h"
#include <cassert>
#include "DirectXCore.h"
#include <imgui.h>

Microsoft::WRL::ComPtr<ID3D12RootSignature> Trail::sRootSignature_;
Microsoft::WRL::ComPtr<ID3D12PipelineState> Trail::sPipelineState_;

void Trail::StaticInitialize()
{
	InitializeGraphicsPipeline();
}

void Trail::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;    // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト


	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/TrailVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,&vsBlob,&errorBlob);
	if ( FAILED(result) )
	{
// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(
			( char* ) errorBlob->GetBufferPointer(),errorBlob->GetBufferSize(),errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/TrailPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,&psBlob,&errorBlob);
	if ( FAILED(result) )
	{
// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(
			( char* ) errorBlob->GetBufferPointer(),errorBlob->GetBufferSize(),errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[ ] = {
	  {// xy座標(1行で書いたほうが見やすい)
	   "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	  {// uv座標(1行で書いたほうが見やすい)
	   "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	   {// uv座標(1行で書いたほうが見やすい)
	   "TEXCOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
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
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
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
	gpipeline.BlendState.RenderTarget[ 0 ] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;                       // 描画対象は1つ
	gpipeline.RTVFormats[ 0 ] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0 = {};
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1,0);	//t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[ 3 ];
	rootparams[ 0 ].InitAsConstantBufferView(0,0,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 1 ].InitAsConstantBufferView(1,0,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 2 ].InitAsDescriptorTable(1,&descRangeSRV0,D3D12_SHADER_VISIBILITY_ALL);


	// スタティックサンプル
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		_countof(rootparams),rootparams,1,&samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1_0,&rootSigBlob,&errorBlob);
	// ルートシグネチャの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateRootSignature(
		0,rootSigBlob->GetBufferPointer(),rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&sRootSignature_));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = sRootSignature_.Get();

	// グラフィックスパイプラインの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
		&gpipeline,IID_PPV_ARGS(&sPipelineState_));
	assert(SUCCEEDED(result));
}

Trail::Trail(uint32_t vertSize)
{
	HRESULT result;
	vertex_.resize(vertSize * 2);
	posArray_.resize(vertSize);
	UINT sizeVB =
		static_cast< UINT >( sizeof(SwordTrailVertex) * vertex_.size() );

	////頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeVB;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = DirectXCore::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapprop,
			D3D12_HEAP_FLAG_NONE,
			&resdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff_)
		);
	assert(SUCCEEDED(result));

	vertBuff_->SetName(L"SWORDTRAIL VERT BUFF");

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;
	vbView_.StrideInBytes = sizeof(vertex_[ 0 ]);

	isVisible_ = false;

	Microsoft::WRL::ComPtr<ID3D12Resource> buff;
	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc2{};

	//定数バッファのヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	resdesc2.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc2.Width = ( sizeof(colorBuffer) + 0xff ) & ~0xff;
	resdesc2.Height = 1;
	resdesc2.DepthOrArraySize = 1;
	resdesc2.MipLevels = 1;
	resdesc2.SampleDesc.Count = 1;
	resdesc2.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = buff->Map(0,nullptr,( void** ) &constMapColor_);
	assert(SUCCEEDED(result));

	constBuffColor_ = buff;

	TexNum = TextureManager::Load("sprite/white1x1.png");
}

void Trail::Update()
{
	if ( isStop_ )
	{
		//先頭の値を配列の後ろへ代入していく
		for ( size_t i = posArray_.size() - 1; i > 0; --i )
		{
			posArray_[ i ] = posArray_[ i - 1 ];
		}

		//配列の先頭に現在の座標を代入
		posArray_.front() = tempPos;
		tempPos = PosBuffer();
	}

	TransferBuff();
}

void Trail::SetPos(const Vector3& head,const Vector3& tail)
{
	tempPos.head = head;
	tempPos.tail = tail;
}

void Trail::SetTexture(const uint32_t& texNum_)
{
	TexNum = texNum_;
}

void Trail::PreDraw()
{
	ID3D12GraphicsCommandList* commandList = DirectXCore::GetInstance()->GetCommandList();

	// パイプラインステートの設定
	commandList->SetPipelineState(sPipelineState_.Get());
	// ルートシグネチャの設定
	commandList->SetGraphicsRootSignature(sRootSignature_.Get());
	// プリミティブ形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

}

void Trail::Draw(const ViewProjection& view)
{
	if ( isVisible_ )
	{

		PreDraw();

		ID3D12GraphicsCommandList* commandList = DirectXCore::GetInstance()->GetCommandList();

		// 頂点データ転送
		commandList->IASetVertexBuffers(0,1,&vbView_);

		// 定数バッファ転送
		commandList->SetGraphicsRootConstantBufferView(
			0,view.constBuff_->GetGPUVirtualAddress());
		// 定数バッファ転送
		commandList->SetGraphicsRootConstantBufferView(
			1,constBuffColor_->GetGPUVirtualAddress());

		// テクスチャ
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList,2,TexNum);

		commandList->DrawInstanced(( UINT ) std::distance(vertex_.begin(),vertex_.end()),1,0,0);
	}
}

void Trail::ResetTrail(const Vector3& resetPos)
{
	PosBuffer reset;
	reset.head = resetPos;
	reset.tail = resetPos;

	for ( size_t i = 0; i < posArray_.size(); i++ )
	{
		posArray_[ i ] = reset;
	}
}

void Trail::SetFirstColor(const Vector3& color)
{
	isStartColor = true;
	FirstColor_ = color;
}

void Trail::SetEndColor(const Vector3& color)
{
	isEndColor = true;
	EndColor_ = color;
}

void Trail::TransferBuff()
{
	HRESULT result;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	SwordTrailVertex* vertMap = nullptr;
	result = vertBuff_->Map(0,nullptr,( void** ) &vertMap);
	assert(SUCCEEDED(result));
	//頂点データを更新する
	float amount = 1.0f / ( posArray_.size() - 1 );
	float v = 0;
	vertex_.clear();
	vertex_.resize(posArray_.size() * 2);
	for ( size_t i = 0,j = 0; i < vertex_.size() && j < posArray_.size(); i += 2,++j )
	{
		Vector3 bias = ( posArray_[ j ].tail - posArray_[ j ].head ) * ( v * 0.5f );

		//頂点座標を二つ代入する
		vertex_[ i ].pos = posArray_[ j ].head + bias;
		vertex_[ i ].uv = Vector2(1.0f,v);
		vertex_[ i + 1 ].pos = posArray_[ j ].tail - bias;
		vertex_[ i + 1 ].uv = Vector2(0.0f,v);

		if ( !isStartColor )
		{
			vertex_[ i ].Color = Vector4(1,1,1,1 - v);
			vertex_[ i + 1 ].Color = Vector4(1,1,1,1 - v);
		}
		else
		{
			Vector4 colorSet = Vector4(FirstColor_.x,FirstColor_.y,FirstColor_.z,1 - v);
			vertex_[ i ].Color = colorSet;
			vertex_[ i + 1 ].Color = colorSet;
		}

		v += amount;
	}
	for ( size_t i = 0; i < vertex_.size(); i++ )
	{
		if ( i > 0 )
		{
			if ( vertex_[ i ].pos.x == 0 &&
				vertex_[ i ].pos.y == 0 &&
				vertex_[ i ].pos.z == 0 )
			{
				vertex_[ i ].pos = vertex_[ i - 1 ].pos;
			}
		}
	}
	std::copy(vertex_.begin(),vertex_.end(),vertMap);

	constMapColor_->color = color_ / 255.0f;
}

void Trail::CreateCurveVertex(std::vector<PosBuffer>& usedPosArray)
{
	usedPosArray;
}
