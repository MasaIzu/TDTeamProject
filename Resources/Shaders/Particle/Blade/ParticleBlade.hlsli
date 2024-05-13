

cbuffer ShaderParameters : register(b0)
{
    float4 StartPos : packoffset(c0);
    matrix mat : packoffset(c1); // 3D�ϊ��s��
    matrix matBillboard : packoffset(c5); //�r���{�[�h�s��
    uint MaxParticleCount : packoffset(c9);
    uint ParticleCount : packoffset(c9.y);
    uint Shot : packoffset(c9.z);
    uint pad : packoffset(c9.w);
    float4 EndPos : packoffset(c10);
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 position : Position;
    float4 color : Color;
    float4 DownColor : DownColor;
    float4 velocity : Velocity;
    float scale : Scale;
    int Frame : Frame; // ���̃p�[�e�B�N�������܂ꂽ�t���[��
    int MaxFrame : MaxFrame; //���̃p�[�e�B�N���̎���
    bool alive : Alive; // ���̃p�[�e�B�N���������Ă��邩�ǂ���
    float MinusAlpha : MinusAlpha;
};

//�W�I���g���V�F�[�_����s�N�Z���V�F�[�_�[�ւ̏o��
struct GSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
	//float3 normal:NORMAL;//�@���x�N�g��
    float2 uv : TEXCOORD; //uv�l
    float4 color : COLOR;
};

struct GpuParticleElement
{
    float4 position;
    float scale;
    float4 color;
    uint isActive; // �����t���O.
    float lifeTime;
    float elapsed;
    float maxLifeTime;
    float4 velocity;
    float4 endPos;
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

float3 lerp(const float3 start, const float3 end, const float t)
{
    return start * (1.0f - t) + end * t;
}

float4 lerp(const float4 start, const float4 end, const float t)
{
    return start * (1.0f - t) + end * t;
}