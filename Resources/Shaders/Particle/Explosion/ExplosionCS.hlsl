#include "Explosion.hlsli"

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
        return;
    }

    uint number = gParticles[index].boostNumber;
    float speed = gParticles[index].speed;
    
    float Div = 1.0f * (gParticles[index].lifeTime / gParticles[index].maxLifeTime);
    
    // 生き残っているパーティクルを動かす.
    float3 velocity = gParticles[index].velocity.xyz;
    float3 position = gParticles[index].position.xyz;
    
    //speed = speed * Div;
    
    velocity *= speed;
    
    position += velocity;
    float4 color = (1,1,1,1);
    
    
    color.x = color.x * Div;
    color.y = color.y * Div;
    color.z = color.z * Div;
    color.w = Div;
    
    //float scale = 0.8f * (gParticles[index].lifeTime / gParticles[index].maxLifeTime);
    //gParticles[index].keepParticleVelocity.xyz += velocity;
    //gParticles[index].scale = scale;
    gParticles[index].position.xyz = position;
    gParticles[index].color = color;

}


ConsumeStructuredBuffer<uint> gFreeIndexList : register(u1);


[numthreads(128, 1, 1)]
void emitParticle(uint3 id : SV_DispatchThreadID)
{
    uint index = id.x;
    
    if (gParticles[index].isActive > 0)
    {
        return;
    }
    uint ParticleCounter = MaxParticleCount / 4;
    if (Shot == 0)
    {
        return;
    }
   
    uint seed = id.x + index * 1235;
    uint indexAdd = index * 1222;
    
    float velSize = 5.0f;
    float particleBoostSize = velSize;
    
    float3 velocity;
    velocity.x = nextRand(indexAdd) * (nextRand1(seed) * particleBoostSize);
    velocity.z = nextRand(indexAdd) * (nextRand1(seed) * particleBoostSize);
    velocity.y = nextRand(indexAdd) * (nextRand1(seed) * particleBoostSize);
    
    velocity = normalize(velocity);
    
    float4 BladeEndPos;
    
    
    float AddTime = 100.0f;
    float TimerMax = 120.0f + AddTime;
    float TimerMin = 70.0f + AddTime;
    
    float LifeTime = Rand1(seed, TimerMax, TimerMin);
    
    
    float speedMax = 10.0f;
    float speedMin = 2.0f;
    float speed = Rand1(indexAdd, speedMax, speedMin) * 0.8f;
    
    float ScaleMax = 12.0f;
    float ScaleMin = 2.0f;
    float scale = Rand1(indexAdd, ScaleMax, ScaleMin);
    
    float4 Position = pos;
    
    uint PosRandMax = 3;
    uint PosRandMin = 1;
    uint PosRand = Rand1(indexAdd, PosRandMax, PosRandMin);
    
    if (PosRand == 1)
    {
        Position.x += 10.0f;
        Position.y += 3.0f;
        Position.y += 5.0f;
    }
    else if (PosRand == 2)
    {
        Position.x -= 5.0f;
        Position.y += 10.0f;
        Position.z -= 10.0f;
    }
    
    
    gParticles[index].isActive = 1;
    gParticles[index].position.xyz = Position.xyz;
    gParticles[index].scale = scale;
    gParticles[index].keepParticleVelocity = float4(0, 0, 0, 0);
    gParticles[index].velocity.xyz = velocity;
    gParticles[index].lifeTime = LifeTime;
    gParticles[index].maxLifeTime = LifeTime;
    gParticles[index].color = float4(1.0f, 1.0f, 1.0f, 1);
    gParticles[index].speed = speed;
    
}
