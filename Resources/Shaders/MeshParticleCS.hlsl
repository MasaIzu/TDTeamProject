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

    //gParticles[index].lifeTime = gParticles[index].lifeTime - dt;
    if (Shot == 2)
    {
        gParticles[index].isActive = 0;
        gDeadIndexList.Append(index);
        return;
    }

  // 生き残っているパーティクルを動かす.
    float3 velocity = gParticles[index].velocity.xyz;
    float3 position = gParticles[index].position.xyz;

    //float4 color = gParticles[index].color;
    //color.w -= 0.005;
    
    //float scale = gParticles[index].scale;
    //scale -= 0.1;
    
    float3 gravity = float3(0, -98.0, 0);
    
    if (Shot == 1)
    {
        position += velocity;
    }
    
    //position += velocity;
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
#if 0
  // フォースの処理.
  float3 center = forceCenter1.xyz;
  float  radius = forceCenter1.w;

  float3 toCenter = center - position.xyz;
  float L = length(toCenter);
  if (L < radius) {
    float velocityLen = length(velocity);
    velocity = reflect(normalize(velocity), -normalize(toCenter));
    velocity *= velocityLen;

    position = radius * -normalize(toCenter) + center;
  }
#endif

    gParticles[index].position.xyz = position;
    //gParticles[index].velocity.xyz = velocity;
    //gParticles[index].color = color;
    //gParticles[index].scale = scale;

}


ConsumeStructuredBuffer<uint> gFreeIndexList : register(u1);


[numthreads(128, 1, 1)]
void emitParticle(uint3 id : SV_DispatchThreadID)
{
    
    if (gParticles[id.x].isActive > 0)
    {
        return;
    }

    uint index;
    index = Rand1(id.x, verticeCount, 0);
    
    float3 Pos = float3(StartPos.xyz);
    
    float3 triangleVertices[3];
    triangleVertices[0] = meshPos[index].pos[0].xyz;
    triangleVertices[1] = meshPos[index].pos[1].xyz;
    triangleVertices[2] = meshPos[index].pos[2].xyz;
    
    // 三角形の法線ベクトルを計算
    float3 edge1 = triangleVertices[1] - triangleVertices[0];
    float3 edge2 = triangleVertices[2] - triangleVertices[0];
    float3 normal = normalize(cross(edge1, edge2));

    // 三角形の面積を計算
    float triangleArea = length(normal);

    // ランダムな重みを生成
    uint seed = id.x;
    float randomWeight1 = nextRand1(seed);
    float randomWeight2 = nextRand1(seed);

    // ランダムなポイントを計算
    float sqrtRandomWeight1 = sqrt(randomWeight1);
    
    //float3 randomPoint = triangleVertices[0] + sqrtRandomWeight1 * (1.0f - randomWeight2) * edge1;//メッシュの線に沿って出る方。綺麗だから使うかも
    
    float3 randomPoint = triangleVertices[0] + sqrtRandomWeight1 * (1.0f - randomWeight2) * edge1 + sqrtRandomWeight1 * randomWeight2 * edge2;
    
    float3 velocity;
    
    float r = nextRand(seed) * 50;
    float theta = nextRand(seed) * 3.14192 * 2.0;
    velocity.x = nextRand(seed) / 4;
    velocity.z = nextRand(seed) / 4;
    velocity.y = (nextRand(seed) / 4);
    
    gParticles[id.x].isActive = 1;
    gParticles[id.x].position.xyz = Pos + randomPoint;
    gParticles[id.x].scale = 0.1;
    gParticles[id.x].velocity.xyz = velocity;
    gParticles[id.x].lifeTime = 1000;
    gParticles[id.x].color = float4(1.0, 1.0, 1.0, 1.0);
    //gParticles[index].colorIndex = floor(nextRand(seed) * 8) % 8;;
}
