#include "ParticleBoost.hlsli"

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
    float4 BladeEndPos = normalize(boostPos.EndPos[number] - boostPos.StartPos[number]);
    BladeEndPos = normalize((boostPos.EndPos[number] + (BladeEndPos * 15)) - gParticles[index].position);
    float3 velocity = gParticles[index].velocity.xyz;
    velocity = lerp(velocity, BladeEndPos.xyz, 0.4f);
    velocity *= speed;
    
    float3 position = boostPos.EndPos[number].xyz + gParticles[index].keepParticleVelocity.xyz;

    position += velocity;

    float scale = 0.8f * (gParticles[index].lifeTime / gParticles[index].maxLifeTime);
    gParticles[index].keepParticleVelocity.xyz += velocity;
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
    else if (Shot == 1)
    {
        if (ParticleCounter > index)
        {
            return;
        }
        else if (ParticleCounter * 2 > index)
        {
            gParticles[index].boostNumber = 1;
        }
        else if (ParticleCounter * 3 > index)
        {
            gParticles[index].boostNumber = 2;
        }
        else if (ParticleCounter * 4 > index)
        {
            return;
        }
    }
    else if (Shot == 2)
    {
        if (ParticleCounter > index)
        {
            gParticles[index].boostNumber = 0;
        }
        else if (ParticleCounter * 2 > index)
        {
            return;
        }
        else if (ParticleCounter * 3 > index)
        {
            return;
        }
        else if (ParticleCounter * 4 > index)
        {
            return;
        }
    }
    else if (Shot == 3)
    {
        if (ParticleCounter > index)
        {
            return;
        }
        else if (ParticleCounter * 2 > index)
        {
            gParticles[index].boostNumber = 1;
        }
        else if (ParticleCounter * 3 > index)
        {
            gParticles[index].boostNumber = 2;
        }
        else if (ParticleCounter * 4 > index)
        {
            return;
        }
    }
    else if (Shot == 4)
    {
        if (ParticleCounter > index)
        {
            return;
        }
        else if (ParticleCounter * 2 > index)
        {
            return;
        }
        else if (ParticleCounter * 3 > index)
        {
            return;
        }
        else if (ParticleCounter * 4 > index)
        {
            gParticles[index].boostNumber = 3;
        }
    }
    else if (Shot == 5)
    {
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
            gParticles[index].boostNumber = 2;
        }
        else if (ParticleCounter * 4 > index)
        {
            return;
        }
    }
    else if (Shot == 6)
    {
        if (ParticleCounter > index)
        {
            return;
        }
        else if (ParticleCounter * 2 > index)
        {
            gParticles[index].boostNumber = 1;
        }
        else if (ParticleCounter * 3 > index)
        {
            gParticles[index].boostNumber = 2;
        }
        else if (ParticleCounter * 4 > index)
        {
            gParticles[index].boostNumber = 3;
        }
    }
    else if (Shot == 7)
    {
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
            gParticles[index].boostNumber = 2;
        }
        else if (ParticleCounter * 4 > index)
        {
            return;
        }
    }
    else if (Shot == 8)
    {
        if (ParticleCounter > index)
        {
            return;
        }
        else if (ParticleCounter * 2 > index)
        {
            gParticles[index].boostNumber = 1;
        }
        else if (ParticleCounter * 3 > index)
        {
            gParticles[index].boostNumber = 2;
        }
        else if (ParticleCounter * 4 > index)
        {
            gParticles[index].boostNumber = 3;
        }
    }
    else if (Shot == 9)
    {
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
            gParticles[index].boostNumber = 2;
        }
        else if (ParticleCounter * 4 > index)
        {
            gParticles[index].boostNumber = 3;
        }
    }
    
    uint seed = id.x + index * 1235;
    uint indexAdd = index * 1222;
    
    float velSize = 0.5f;
    float particleBoostSize = velSize + (boostPower - 0.1f) * 2.0f;
    
    float3 velocity;
    velocity.x = nextRand(indexAdd) * (nextRand1(seed) * particleBoostSize);
    velocity.z = nextRand(indexAdd) * (nextRand1(seed) * particleBoostSize);
    velocity.y = nextRand(indexAdd) * (nextRand1(seed) * particleBoostSize);
    
    float4 BladeEndPos;
    
    float TimerMax = 30.0f;
    float TimerMin = 10.0f;
    
    float LifeTime = Rand1(seed, TimerMax, TimerMin);
    
    float speed = (TimerMax / LifeTime) * boostPower;
    
    gParticles[index].isActive = 1;
    gParticles[index].position.xyz = float3(boostPos.EndPos[gParticles[index].boostNumber].xyz);
    gParticles[index].scale = 0.8f;
    gParticles[index].keepParticleVelocity = float4(0, 0, 0, 0);
    gParticles[index].velocity.xyz = velocity;
    gParticles[index].lifeTime = LifeTime;
    gParticles[index].maxLifeTime = LifeTime;
    gParticles[index].color = float4(0.05, 0.05, 0.8, 0.5);
    gParticles[index].speed = speed;
    
}
