
cbuffer ViewProjection : register(b0)
{
    matrix view; // �r���[�ϊ��s��
    matrix projection; // �v���W�F�N�V�����ϊ��s��
    float3 cameraPos; // �J�������W�i���[���h���W�j
};

cbuffer ConstantBufferColor : register(b1)
{
    float4 colorA;
};

//���_�V�F�[�_�̏o�͍\����
//�i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����j
struct VSOutput {
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	//uv���W
	float2 uv :TEXCOORD;
    //color
    float4 color : TEXCOLOR;
};