#include "Particle.hlsli"


struct GpuParticleElement
{
    float4 position;
    float scale;
    float4 color;
    uint isActive; // 生存フラグ.
    float lifeTime;
    float elapsed;
    uint colorIndex;
    float4 velocity;
};

RWStructuredBuffer<GpuParticleElement> gParticles : register(u0);

struct PSInput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

VSOutput main(uint vertexId : SV_VertexId)
{
    VSOutput output = (VSOutput) 0; // ピクセルシェーダーに渡す値
    if (vertexId >= MaxParticleCount)
    {
        return output;
    }
    if (gParticles[vertexId].isActive == 0)
    {
        return output;
    }

    float4 position = gParticles[vertexId].position;
    uint colorIndex = gParticles[vertexId].colorIndex;
    position.w = 1;
    output.position = position;
    output.scale = gParticles[vertexId].scale;
    output.color = gParticles[vertexId].color;
	return output;
}