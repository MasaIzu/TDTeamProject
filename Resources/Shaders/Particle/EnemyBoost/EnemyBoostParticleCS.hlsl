#include "EnemyBoostParticle.hlsli"

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

    uint number = gParticles[index].boostNumber;
    float speed = gParticles[index].speed;
    // 生き残っているパーティクルを動かす.
    float3 position = boostPos.StartPos[number].xyz + gParticles[index].keepParticleVelocity.xyz;
    float4 BoostEndPos = normalize(boostPos.EndPos[number] - boostPos.StartPos[number]) * 30;
    BoostEndPos = boostPos.StartPos[number] + BoostEndPos;
    float3 LookVelocity = normalize(BoostEndPos.xyz - position);
    float3 velocity = gParticles[index].velocity.xyz;
    velocity = lerp(velocity, LookVelocity, 0.2f);
    
    velocity *= speed;
    position += velocity;
    

    float scale = 0.8f * (gParticles[index].lifeTime / gParticles[index].maxLifeTime);
    gParticles[index].keepParticleVelocity.xyz += velocity;
    gParticles[index].velocity.xyz = velocity;
    gParticles[index].scale = scale;
    gParticles[index].position.xyz = position;
    gParticles[index].velocity.xyz = velocity;
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
    
    
    if (ParticleCounter > index)
    {
        gParticles[index].boostNumber = 0;
    }
    else if (ParticleCounter * 2 > index)
    {
        gParticles[index].boostNumber = 1;
    }
    else if (ParticleCounter * 3 > index)
    {
        gParticles[index].boostNumber = 0;
    }
    else if (ParticleCounter * 4 > index)
    {
        gParticles[index].boostNumber = 1;
    }
    
    
    uint seed = id.x + index * 1235;
    uint indexAdd = index * 1222;
        
    float3 velocity;
    velocity.x = nextRand(indexAdd) * (nextRand1(seed));
    velocity.z = nextRand(indexAdd) * (nextRand1(seed));
    velocity.y = nextRand(indexAdd) * (nextRand1(seed));
    
    velocity = normalize(velocity);
    
    float TimerMax = 150.0f;
    float TimerMin = 60.0f;
    float LifeTime = Rand1(indexAdd, TimerMax, TimerMin) * nextRand1(seed);
    
    float speed = (1.1f - (LifeTime / TimerMax)) * 0.9f;
    
    gParticles[index].isActive = 1;
    gParticles[index].position.xyz = float3(boostPos.StartPos[gParticles[index].boostNumber].xyz);
    gParticles[index].scale = 0.8f;
    gParticles[index].keepParticleVelocity = float4(0, 0, 0, 0);
    gParticles[index].velocity.xyz = velocity;
    gParticles[index].lifeTime = LifeTime;
    gParticles[index].maxLifeTime = LifeTime;
    gParticles[index].color = float4(0.2, 0.05, 0.05, 0.5);
    gParticles[index].speed = speed;
    
}
