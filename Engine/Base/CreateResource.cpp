#include "CreateResource.h"
#include "DirectXCore.h"
#include <cassert>


MyFunction::ComPtr<ID3D12Resource1> MyFunction::CreateResource(const CD3DX12_RESOURCE_DESC& desc,const D3D12_RESOURCE_STATES& resourceStates, const D3D12_CLEAR_VALUE* clearValue,const D3D12_HEAP_TYPE& heapType)
{
    HRESULT hr;
    ComPtr<ID3D12Resource1> ret;
    const auto heapProps = CD3DX12_HEAP_PROPERTIES(heapType);
    hr = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        resourceStates,
        clearValue,
        IID_PPV_ARGS(&ret)
    );
    assert(SUCCEEDED(hr));
    return ret;
}

void MyFunction::WriteToUploadHeapMemory(ID3D12Resource1* resource,const uint32_t& size, const void* data)
{
    void* mapped;
    HRESULT hr = resource->Map(0, nullptr, &mapped);
    if (SUCCEEDED(hr))
    {
        memcpy(mapped, data, size);
        resource->Unmap(0, nullptr);
    }
}

