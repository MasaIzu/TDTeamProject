
cbuffer ShaderViewParameters : register(b0)
{
    matrix mat : packoffset(c0); // 3D�ϊ��s��
    matrix matBillboard : packoffset(c4); //�r���{�[�h�s��
};

cbuffer ShaderDetailParameters : register(b1)
{
    float4 StartPos : packoffset(c0);
    float4 EndPos : packoffset(c1);
    float4 StartColor : packoffset(c2);
    float4 EndColor : packoffset(c3);
    float4 Angle : packoffset(c4);
    uint Shot : packoffset(c5);
    uint EndPointActive : packoffset(c5.y);
    uint RandomVelocity : packoffset(c5.z);
    uint RandomLife : packoffset(c5.w);
    uint RandomSpeed : packoffset(c6);
    uint RandomScale : packoffset(c6.y);
    float Speed : packoffset(c6.z);
    float LerpStrength : packoffset(c6.w);
    float Scale : packoffset(c7);
    float ScaleTinker : packoffset(c7.y);
    float MaxLife : packoffset(c7.z);
    uint MaxParticleCount : packoffset(c7.w);
    uint AdditiveSynthesis : packoffset(c8);
    uint isLoad : packoffset(c8.y);
    float2 RandomLifeMinMax : packoffset(c8.z);
    float2 RandomSpeedMinMax : packoffset(c9);
    float2 RandomScaleMinMax : packoffset(c9.z);
    float SpeedDivideSize : packoffset(c10);
    float ScaleDivideSize : packoffset(c10.y);
    float GravityStrength : packoffset(c10.z);
    uint Interlocking : packoffset(c10.w);
    float InterlockingStrength : packoffset(c11);
    float InterlockingLength : packoffset(c11.y);
    uint ScaleDownLifeTime : packoffset(c11.z);
    uint EmitParticles : packoffset(c11.w);
    uint ParticleGroup : packoffset(c12);
    uint ParticleGroupCount : packoffset(c12.y);
    float GroupTimer : packoffset(c12.z);
    float MaxGroupTimer : packoffset(c12.w);
    float2 RandomGroupTimerMinMax : packoffset(c13);
    uint RandomParticleExplosion : packoffset(c13.z);
    uint ShapeNumber : packoffset(c13.w);
    float Width : packoffset(c14);
    float Height : packoffset(c14.y);
    float Depth : packoffset(c14.z);
    uint CollisionON : packoffset(c14.w);
    float4 ShapeScale : packoffset(c15);
    float3 VelocityAdjustment : packoffset(c16);
    uint GettingUpDownScale : packoffset(c16.w);
    float PostEffectPow : packoffset(c17);
};


static const int MaxColCount = 30;

cbuffer ShaderDetailCollision : register(b2)
{
    float4 ColPos[MaxColCount];
    float4 ColScale[MaxColCount];
    uint ColCount;
}

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
    uint alive : Alive; // ���̃p�[�e�B�N���������Ă��邩�ǂ���
    float MinusAlpha : MinusAlpha;
};

//�W�I���g���V�F�[�_����s�N�Z���V�F�[�_�[�ւ̏o��
struct GSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
	//float3 normal:NORMAL;//�@���x�N�g��
    float2 uv : TEXCOORD; //uv�l
    float4 color : COLOR;
    float4 PostEffectPowColor : COLORPOW;
};

struct GpuParticleElement
{
    float4 position;
    float4 color;
    float4 velocity;
    uint isActive; // �����t���O.
    float lifeTime;
    float MaxLifeTime;
    float scale;
    float Speed;
    float graceOfTime;
    float ScaleKeep;
    uint GroupNumber;
    float GroupTimer;
    float MaxGroupTimer;
    float PostEffectPow;
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

uint Rand(uint seed, int max, int min)
{
    seed = (seed * 1664525u + 1013904223u) & 0xFFFFFFFF;
    float normalized = float(seed) / 4294967296.0; // ���K���i0.0 �` 1.0�j
    return uint(min + (max - min) * normalized);
}

float Rand1(uint seed, float max, float min)
{
    seed = (seed * 1664525u + 1013904223u) & 0xFFFFFFFF;
    float normalized = float(seed) / 4294967296.0; // ���K���i0.0 �` 1.0�j
    float result = min + (max - min) * normalized;
    return result;
}

// �^�������_�����𐶐�����֐�
float RandFloat2(float2 co)
{
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}

// �����_���ȍ��W�𐶐�����֐��iY���W���܂ށj
float3 GenerateRandomPoint(uint seed, uint seed2, uint seed3, float3 center, float width, float height, float depth)
{
    // �e���W�̂��߂̃����_���l�𐶐�
    float randomX = Rand1(seed, center.x + width / 2.0, center.x - width / 2.0);
    float randomY = Rand1(seed2, center.y + height / 2.0, center.y - height / 2.0);
    float randomZ = Rand1(seed3, center.z + depth / 2.0, center.z - depth / 2.0);
    
    // �������ꂽ�����_���ȍ��W��Ԃ�
    return float3(randomX, randomY, randomZ);
}

// �e�g���w�h�������̃����_���ȓ_�𐶐�����֐�
float3 GenerateRandomPointInTetrahedron(uint seed, uint seed2, uint seed3, uint seed4, float3 A, float3 B, float3 C, float3 D)
{
    float r1 = Rand1(seed, 1.0, 0.0);
    float r2 = Rand1(seed2, 1.0, 0.0);
    float r3 = Rand1(seed3, 1.0, 0.0);
    float r4 = Rand1(seed4, 1.0, 0.0);

    // �����_���Ȑ��̍��v�Ő��K��
    float sum = r1 + r2 + r3 + r4;
    r1 /= sum;
    r2 /= sum;
    r3 /= sum;
    r4 /= sum;
    
    // �o���Z���g���b�N���W���g�p���ē_P�̍��W���v�Z
    return A * r1 + B * r2 + C * r3 + D * r4;
}

float cubicRoot(float x)
{
    return pow(x, 1.0 / 3.0);
}

// �P�ʋ����̃����_���ȓ_�𐶐�����֐�
float3 RandomPointInSphere(float radius, float3 seed)
{
    float u = RandFloat2(seed.xy);
    float v = RandFloat2(seed.yz);
    float theta = u * 2.0 * 3.14159265; // 0 ���� 2��
    float phi = acos(2.0 * v - 1.0); // 0 ���� ��

    // �����W�n���璼�����W�n�ւ̕ϊ�
    float x = sin(phi) * cos(theta);
    float y = sin(phi) * sin(theta);
    float z = cos(phi);

    return float3(x, y, z) * radius; // �P�ʋ��̃����_���ȓ_
}

float3 RandomVec3(uint SEED, int MAX, int MIN)
{
    float3 randomVec;
    
    randomVec.x = Rand(SEED, MAX, MIN);
    randomVec.y = Rand(SEED, MAX, MIN);
    randomVec.z = Rand(SEED, MAX, MIN);
    
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

float4 QuaternionFromAxisAngle(float3 axis, float angle)
{
    float halfAngle = angle * 0.5;
    float s = sin(halfAngle);

    float4 quaternion;
    quaternion.w = cos(halfAngle);
    quaternion.xyz = axis * s;
    return quaternion;
}

// �l�����̐ς��v�Z����֐�
float4 QuaternionMultiply(float4 q1, float4 q2)
{
    return float4(q1.w * q2.xyz + q2.w * q1.xyz + cross(q1.xyz, q2.xyz),
                  q1.w * q2.w - dot(q1.xyz, q2.xyz));
}

// �l�����ɂ��x�N�g���̉�]
float3 RotateVectorByQuaternion(float3 v, float4 q)
{
    float4 vQuat = float4(v, 0.0); // �x�N�g�����l�����Ɋg��
    float4 qConj = float4(-q.xyz, q.w); // �l�����̋���

    // �l�����̐ς��v�Z���ăx�N�g���ɉ�]��K�p
    float4 rotatedQuat = QuaternionMultiply(QuaternionMultiply(q, vQuat), qConj);

    return rotatedQuat.xyz; // ��]���ꂽ�x�N�g����Ԃ�
}