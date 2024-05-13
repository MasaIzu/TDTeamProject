
struct ParticleCount
{
    uint UseCount : ParticleUseCount;
    uint StartIndex : ParticleStartIndex;
    uint2 pad : Pad;
};

struct MeshPos
{
    float4 pos[3] : MeshPos;
};

cbuffer ShaderParameters : register(b0)
{
    float4 StartPos : packoffset(c0);
    matrix mat : packoffset(c1); // 3D変換行列
    matrix matBillboard : packoffset(c5); //ビルボード行列
    uint MaxParticleCount : packoffset(c9);
    uint verticeCount : packoffset(c9.y);
    uint Shot : packoffset(c9.z);
    uint pad : packoffset(c9.w);
    MeshPos meshPos[1000] : packoffset(c10);
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 position : Position;
    float scale : Scale;
    float4 color : Color;
    float4 DownColor : DownColor;
    float4 velocity : Velocity;
    float3 FinalVelocity : FinalVelocity;
    int Frame : Frame; // このパーティクルが生まれたフレーム
    int MaxFrame : MaxFrame; //このパーティクルの寿命
    bool alive : Alive; // このパーティクルが生きているかどうか
    float MinusAlpha : MinusAlpha;
};

//ジオメトリシェーダからピクセルシェーダーへの出力
struct GSOutput
{
	float4 svpos:SV_POSITION;//システム用頂点座標
	//float3 normal:NORMAL;//法線ベクトル
	float2 uv:TEXCOORD;//uv値
	float4 color:COLOR;
};

uint wang_hash(uint seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

float Rand1(uint SEED, int MAX, int MIN)
{
    uint rand = wang_hash(SEED * 1847483629);
    float result;
    result = (rand % 1024) / 1024.0f;
    result = (MAX + abs(MIN)) * result - abs(MIN);
    return result;
}

float3 RandomVec3(uint SEED, int MAX, int MIN)
{
    float3 randomVec;
    
    randomVec.x = Rand1(SEED, MAX, MIN);
    randomVec.y = Rand1(SEED, MAX, MIN);
    randomVec.z = Rand1(SEED, MAX, MIN);
    
    return randomVec;
}

float nextRand(inout uint s)
{
    s = (1664525u * s + 1013904223u);
    return saturate(float(s & 0x00FFFFFF) / float(0x01000000)) - 0.5;
}

float nextRand1(inout uint s)
{
    s = (1664525u * s + 1013904223u);
    return saturate(float(s & 0x00FFFFFF) / float(0x01000000));
}