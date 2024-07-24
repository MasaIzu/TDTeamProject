
cbuffer ViewProjection : register(b0)
{
    matrix view; // �r���[�ϊ��s��
    matrix projection; // �v���W�F�N�V�����ϊ��s��
    float3 cameraPos; // �J�������W�i���[���h���W�j
};

cbuffer ConstantBufferColor : register(b1)
{
    matrix matBillboard;
};

//�W�I���g���V�F�[�_����s�N�Z���V�F�[�_�[�ւ̏o��
struct GSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float2 uv : TEXCOORD; //uv�l
    float4 color : COLOR;
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
    //�T�C�Y
    float size : FIRSTSIZE;

};

float GetYFloatAngle(const float x, const float y)
{
    float PI = 3.1415926535897931f;
    
    float angle = atan2(x, y);

    if (angle < 0)
    {
        angle = angle + 2 * PI;
    }

    angle = floor(angle * 360 / (2 * PI));

    return angle;
}

matrix Rot(float3 angle)
{
    float x, y, z, w;
    
    float cosR = cos(angle.x / 2.f);
    float sinR = sin(angle.x / 2.f);
    float cosP = cos(angle.y / 2.f);
    float sinP = sin(angle.y / 2.f);
    float cosY = cos(angle.z / 2.f);
    float sinY = sin(angle.z / 2.f);

    y = cosR * sinP * cosY + sinR * cosP * sinY;
    z = cosR * cosP * sinY - sinR * sinP * cosY;
    x = sinR * cosP * cosY - cosR * sinP * sinY;
    w = cosR * cosP * cosY + sinR * sinP * sinY;
    
    float xx = x * x * 2.0f;
    float yy = y * y * 2.0f;
    float zz = z * z * 2.0f;
    float xy = x * y * 2.0f;
    float xz = x * z * 2.0f;
    float yz = y * z * 2.0f;
    float wx = w * x * 2.0f;
    float wy = w * y * 2.0f;
    float wz = w * z * 2.0f;

    matrix result =
    {
        1.0f - yy - zz, xy + wz, xz - wy, 0.0f,
		       xy - wz, 1.0f - xx - zz, yz + wx, 0.0f,
		       xz + wy, yz - wx, 1.0f - xx - yy, 0.0f,
	  	          0.0f, 0.0f, 0.0f, 1.0f
    };

    return result;
}

//�x�N�g���ƍs��̊|���Z(�o��Vector3)
float3 MatVector(const matrix matrix4, const float4 vector3)
{
    float3 matVector = { 0, 0, 0 };

    matVector.x = vector3.x * matrix4[0][0] + vector3.y * matrix4[1][0] + vector3.z * matrix4[2][0];
    matVector.y = vector3.x * matrix4[0][1] + vector3.y * matrix4[1][1] + vector3.z * matrix4[2][1];
    matVector.z = vector3.x * matrix4[0][2] + vector3.y * matrix4[1][2] + vector3.z * matrix4[2][2];

    float4 a = mul(matrix4, vector3);
    
    return matVector;
}