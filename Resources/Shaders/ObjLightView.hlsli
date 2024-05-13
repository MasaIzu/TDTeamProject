cbuffer WorldTransform : register(b0)
{
    matrix world; // ���[���h�s��
    float m_alpha; // �A���t�@
    uint shadowMapFlag; //�V���h�E�p
};


cbuffer ViewProjection : register(b1)
{
    matrix view; // �r���[�ϊ��s��
    matrix projection; // �v���W�F�N�V�����ϊ��s��
    float3 cameraPos; // �J�������W�i���[���h���W�j
};


cbuffer Material : register(b2)
{
    float3 m_ambient : packoffset(c0); // �A���r�G���g�W��
    float3 m_diffuse : packoffset(c1); // �f�B�t���[�Y�W��
    float3 m_specular : packoffset(c2); // �X�y�L�����[�W��
}

// �_�����̐�
static const int POINTLIGHT_MAXNUM = 30;

struct PointLight
{
    float3 lightpos; // ���C�g���W
    float3 lightcolor; // ���C�g�̐F(RGB)
    float3 lightatten; // ���C�g���������W��
    uint active;
};

// �X�|�b�g���C�g�̐�
static const int SPOTLIGHT_MAXNUM = 30;

struct SpotLight
{
    float3 lightv; // ���C�g�̌��������̋t�x�N�g���i�P�ʃx�N�g���j
    float3 lightpos; // ���C�g���W
    float3 lightcolor; // ���C�g�̐F(RGB)
    float3 lightatten; // ���C�g���������W��
    float2 lightfactoranglecos; // ���C�g�����p�x�̃R�T�C��
    uint active;
};

// �ۉe�̐�
static const int CIRCLESHADOW_MAXNUM = 3;

struct CircleShadow
{
    float3 dir; // ���e�����̋t�x�N�g���i�P�ʃx�N�g���j
    float3 casterPos; // �L���X�^�[���W
    float distanceCasterLight; // �L���X�^�[�ƃ��C�g�̋���
    float3 atten; // ���������W��
    float2 factorAngleCos; // �����p�x�̃R�T�C��
    uint active;
};

cbuffer LightGroup : register(b3)
{
    float3 ambientColor;
    uint flag;
	
    uint pointLightCount;
    uint spotLightCount;
    uint circleShadowCount;
    float pad2;
	
    PointLight pointLights[POINTLIGHT_MAXNUM];
    SpotLight spotLights[SPOTLIGHT_MAXNUM];
    CircleShadow circleShadows[CIRCLESHADOW_MAXNUM];
}

cbuffer PolygonExplosion : register(b4)
{
    float _Destruction : packoffset(c0);
    float _ScaleFactor : packoffset(c0.y);
    float _RotationFactor : packoffset(c0.z);
    float _PositionFactor : packoffset(c0.w);
}

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float4 worldpos : POSITION; // ���[���h���W
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD; // uv�l
};
