#include "ParticleBlade.hlsli"

RWStructuredBuffer<GpuParticleElement> gParticles : register(u0);
AppendStructuredBuffer<uint> gDeadIndexList : register(u1);

[numthreads(128, 1, 1)]
void initParticle(uint3 id : SV_DispatchThreadID)
{
    if (id.x < MaxParticleCount)
    {
        uint index = id.x;
        gParticles[index].isActive = 0;

        gDeadIndexList.Append(index);
    }
}

[numthreads(128, 1, 1)]
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
    float4 BladeEndPos = normalize(EndPos - StartPos);
    BladeEndPos = normalize((StartPos + (BladeEndPos * 15)) - gParticles[index].position);
    float3 velocity = gParticles[index].velocity.xyz;
    velocity = lerp(velocity, BladeEndPos.xyz, 0.4f);
    
    
    float3 position = gParticles[index].position.xyz;

    position += velocity;

    float scale = 0.8f * (gParticles[index].lifeTime / gParticles[index].maxLifeTime);
    gParticles[index].scale = scale;
    gParticles[index].position.xyz = position;
    gParticles[index].velocity.xyz = velocity;
}


ConsumeStructuredBuffer<uint> gFreeIndexList : register(u1);


[numthreads(128, 1, 1)]
void emitParticle(uint3 id : SV_DispatchThreadID)
{
    uint index = id.x;
    
    if (Shot == 0)
    {
        return;
    }
    
    if (gParticles[index].isActive > 0)
    {
        return;
    }
        
    uint seed = id.x + index * 1235;

    uint indexAdd = index * 1222;
    
    float3 velocity;

    velocity.x = nextRand(indexAdd) + nextRand1(seed) * 2;
    velocity.z = nextRand(indexAdd) + nextRand1(seed) * 2;
    velocity.y = nextRand(indexAdd) + nextRand1(seed) * 2;
    
    float4 BladeEndPos = normalize(EndPos - StartPos);
    BladeEndPos = (BladeEndPos * 15);
    
    float LifeTime = Rand1(seed,19,10);
    
    //velocity = velocity / LifeTime;
    //velocity += BladeEndPos.xyz / LifeTime;
    
    gParticles[index].isActive = 1;
    gParticles[index].position.xyz = float3(StartPos.xyz);
    gParticles[index].scale = 0.8f;
    gParticles[index].velocity.xyz = velocity;
    gParticles[index].lifeTime = LifeTime;
    gParticles[index].maxLifeTime = LifeTime;
    gParticles[index].color = float4(1, 0.05, 0.05, 0.5);
    gParticles[index].endPos = StartPos + BladeEndPos;
    //gParticles[index].colorIndex = floor(nextRand(seed) * 8) % 8;;
}
