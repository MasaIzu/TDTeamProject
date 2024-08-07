#include "Mesh.h"
#include <cassert>
#include <d3dcompiler.h>
#include <DescHeapSRV.h>
#include <ShadowMap.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

void Mesh::SetName(const std::string& Name_) { this->name_ = Name_; }

void Mesh::AddVertex(const VertexPosNormalUv& vertex) { vertices_.emplace_back(vertex); }

void Mesh::AddIndex(const unsigned short& index) { indices_.emplace_back(index); }

void Mesh::AddSmoothData(const unsigned short& indexPosition,const unsigned short& indexVertex) {
	smoothData_[indexPosition].emplace_back(indexVertex);
}


void Mesh::CalculateSmoothedVertexNormals() {
	auto itr = smoothData_.begin();
	for (; itr != smoothData_.end(); ++itr) {
		// 各面用の共通頂点コレクション
		std::vector<unsigned short>& v = itr->second;
		// 全頂点の法線を平均する
		XMVECTOR normal = {};
		for (unsigned short index : v) {
			normal += XMVectorSet(
				vertices_[index].normal.x, vertices_[index].normal.y, vertices_[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());

		for (unsigned short index : v) {
			vertices_[index].normal = { normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
		}
	}
}

void Mesh::SetMaterial(Material* material) { this->material_ = material; }

void Mesh::CreateBuffers() {
	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices_.size());

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// 頂点バッファ生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// 頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices_.begin(), vertices_.end(), vertMap);
		vertBuff_->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;
	vbView_.StrideInBytes = sizeof(vertices_[0]);

	if (FAILED(result)) {
		assert(0);
		return;
	}

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices_.size());
	// リソース設定
	resourceDesc.Width = sizeIB;
	// インデックスバッファ生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&indexBuff_));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices_.begin(), indices_.end(), indexMap);
		indexBuff_->Unmap(0, nullptr);
	}

	// インデックスバッファビューの作成
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;


	// リソース設定
	CD3DX12_RESOURCE_DESC BoneResourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(Bone) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &BoneResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&BoneBuff_));

	// 定数バッファとのデータリンク
	result = BoneBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));


}

void Mesh::Draw(
	ID3D12GraphicsCommandList* commandList,const UINT& rooParameterIndexMaterial,
	const UINT& rooParameterIndexTexture,const uint32_t& textureIndex) {
	// 頂点バッファをセット
	commandList->IASetVertexBuffers(0, 1, &vbView_);
	// インデックスバッファをセット
	commandList->IASetIndexBuffer(&ibView_);

	// マテリアルのグラフィックスコマンドをセット
	material_->SetGraphicsCommand(commandList, rooParameterIndexMaterial, rooParameterIndexTexture, textureIndex);

	// 描画コマンド
	commandList->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void Mesh::Draw(ID3D12GraphicsCommandList* commandList,const UINT& rooParameterIndexMaterial,const UINT& rooParameterIndexTexture,const uint32_t& textureIndex,const uint32_t& shadowMapTextureIndex)
{
	// 頂点バッファをセット
	commandList->IASetVertexBuffers(0,1,&vbView_);
	// インデックスバッファをセット
	commandList->IASetIndexBuffer(&ibView_);

	// マテリアルのグラフィックスコマンドをセット
	material_->SetGraphicsCommand(commandList,rooParameterIndexMaterial,rooParameterIndexTexture,textureIndex);

	ShadowMap::SetGraphicsRootDescriptorTable(commandList,shadowMapTextureIndex);

	// 描画コマンド
	commandList->DrawIndexedInstanced(( UINT ) indices_.size(),1,0,0,0);
}

//void Mesh::Draw(ID3D12GraphicsCommandList* commandList,const UINT& rooParameterIndexMaterial,const UINT& rooParameterIndexTexture,const uint32_t& textureIndex,const UINT& rooParameterShadowIndexTexture,const uint32_t& shadowMapTextureIndex)
//{
//	// 頂点バッファをセット
//	commandList->IASetVertexBuffers(0,1,&vbView_);
//	// インデックスバッファをセット
//	commandList->IASetIndexBuffer(&ibView_);
//
//
//	// マテリアルのグラフィックスコマンドをセット
//	material_->SetGraphicsCommand(commandList,rooParameterIndexMaterial,rooParameterIndexTexture,textureIndex);
//
//	// 描画コマンド
//	commandList->DrawIndexedInstanced(( UINT ) indices_.size(),1,0,0,0);
//}


void Mesh::SetLight(const Vector3& ambient,const Vector3& diffuse,const Vector3& specular,const float& alpha) {
	material_->SetLight(ambient, diffuse, specular, alpha);
}