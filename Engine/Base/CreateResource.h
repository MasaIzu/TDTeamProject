#pragma once

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <d3dx12.h>
#include <vector>
MY_SUPPRESS_WARNINGS_END

/// <summary>
/// リソース生成
/// </summary>
namespace MyFunction {

    template<class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    static const UINT FrameBufferCount = 2;

    // リソース生成
    ComPtr<ID3D12Resource1> CreateResource(
        const CD3DX12_RESOURCE_DESC& desc,
		const D3D12_RESOURCE_STATES& resourceStates,
        const D3D12_CLEAR_VALUE* clearValue,
		const D3D12_HEAP_TYPE& heapType
    );
	//Mapする
    void WriteToUploadHeapMemory(ID3D12Resource1* resource,const uint32_t& size, const void* data);
}