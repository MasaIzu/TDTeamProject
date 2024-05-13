#include "BulletSmokeParticle.hlsli"

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
    
    if (gParticles[index].keepTime >= 0)
    {
        gParticles[index].keepTime -= dt;
        return;
    }

    gParticles[index].lifeTime = gParticles[index].lifeTime - dt;
    if (gParticles[index].lifeTime <= 0)
    {
        gParticles[index].isActive = 0;
        gDeadIndexList.Append(index);
        return;
    }

    float speed = gParticles[index].speed;
    
    float Div = 1.0f * (gParticles[index].lifeTime / gParticles[index].maxLifeTime);
    
    // 生き残っているパーティクルを動かす.
    float3 velocity = gParticles[index].velocity.xyz;
    float3 position = gParticles[index].position.xyz;
    
    speed = speed * Div;
    
    velocity *= speed;
    
    position += velocity;
    float4 color = gParticles[index].color;
    
    
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
    if (Shot == 0)
    {
        return;
    }
    if (makePermissionCount < index)
    {
        return;
    }
    
    uint seed = id.x + index * 1235;
    uint indexAdd = index * 1222;
    uint indexDiv = index / 2;
    
    float velSize = 5.0f;
    float particleBoostSize = velSize;
    
    float3 velocity;
    velocity.x = nextRand(indexAdd) * (nextRand1(seed) * particleBoostSize);
    velocity.z = nextRand(indexAdd) * (nextRand1(seed) * particleBoostSize);
    velocity.y = nextRand(indexAdd) * (nextRand1(seed) * particleBoostSize);
    
    velocity = normalize(velocity);
    
    float4 BladeEndPos;
    
    float TimerMax = 120.0f;
    float TimerMin = 100.0f;
    
    float LifeTime = Rand1(seed, TimerMax, TimerMin);
    
    
    float speedMax = 5.0f;
    float speedMin = 1.0f;
    float speed = Rand1(indexAdd, speedMax, speedMin) / 30.0f;
    
    float ScaleMax = 40.0f;
    float ScaleMin = 10.0f;
    float scale = 2.0f;
    
    float colorMax = 100.0f;
    float colorMin = 1.0f;
    float red = 1.0f;
    float green = 1.0f;
    float blue = 1.0f;
    
    float4 Position = pos;
    
    gParticles[index].keepTime = 5;
    gParticles[index].isActive = 1;
    gParticles[index].position.xyz = Position.xyz;
    gParticles[index].scale = scale;
    gParticles[index].keepParticleVelocity = float4(0, 0, 0, 0);
    gParticles[index].velocity.xyz = velocity;
    gParticles[index].lifeTime = LifeTime;
    gParticles[index].maxLifeTime = LifeTime;
    gParticles[index].color = float4(red, green, blue, 1);
    gParticles[index].speed = speed;
    
}