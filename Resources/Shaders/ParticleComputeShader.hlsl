#include "Particle.hlsli"
#include "Easing.hlsli"


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
AppendStructuredBuffer<uint> gDeadIndexList : register(u1);

[numthreads(32, 1, 1)]
void initParticle(uint3 id : SV_DispatchThreadID)
{
    if (id.x < MaxParticleCount)
    {
        uint index = id.x;
        gParticles[index].isActive = 0;

        gDeadIndexList.Append(index);
    }
}

[numthreads(32, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    uint index = id.x;
    if (index >= MaxParticleCount)
    {
        return;
    }
    if (gParticles[index].isActive == 0)
    {
        return;
    }
    const float dt = 1;

    gParticles[index].lifeTime = gParticles[index].lifeTime - dt;
    if (gParticles[index].lifeTime <= 0)
    {
        gParticles[index].isActive = 0;
        gDeadIndexList.Append(index);
        return;
    }

  // 生き残っているパーティクルを動かす.
    float3 velocity = gParticles[index].velocity.xyz;
    float3 position = gParticles[index].position.xyz;

    float3 gravity = float3(0, -98.0, 0);
    position += velocity;
    //velocity += gravity * dt;

    //if (position.y < 0)
    //{
    //    velocity.y *= -1;
    //    velocity *= 0.6;
    //    position.y = 0;

    //    if (length(velocity) < 0.001)
    //    {
    //        velocity = 0;
    //    }
    //}


    gParticles[index].position.xyz = position;
    //gParticles[index].velocity.xyz = velocity;
}


ConsumeStructuredBuffer<uint> gFreeIndexList : register(u1);

[numthreads(32, 1, 1)]
void emitParticle(uint3 id : SV_DispatchThreadID)
{
    uint index = gFreeIndexList.Consume();
    if (gParticles[index].isActive > 0)
    {
        return;
    }

    float a = index;
    
    uint seed = id.x + index * 1235;

    float3 velocity;
    
    float r = nextRand(seed) * 50;
    float theta = nextRand(seed) * 3.14192 * 2.0;
    velocity.x = r * cos(theta);
    velocity.z = r * sin(theta);
    velocity.y = nextRand(seed) * 1;

    gParticles[index].isActive = 1;
    gParticles[index].position.xyz = float3(StartPos.xyz);
    gParticles[index].scale = 1.0f;
    gParticles[index].velocity.xyz = velocity;
    gParticles[index].lifeTime = 100;
    gParticles[index].color = float4(1, 1, 1, 1);
    //gParticles[index].colorIndex = floor(nextRand(seed) * 8) % 8;;
}
