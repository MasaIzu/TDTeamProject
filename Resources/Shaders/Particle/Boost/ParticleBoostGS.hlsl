#include "ParticleBoost.hlsli"

static const int vnum = 4;

//センターからのオフセット
static const float4 offset_array[vnum] =
{
	float4(-0.3f,-0.3f,0,0),//左下
	float4(-0.3f,+0.3f,0,0),//左上
	float4(+0.3f,-0.3f,0,0),//右下
	float4(+0.3f,+0.3f,0,0)//右上
};

static const float2 uv_array[vnum] =
{
	float2(0,1),
	float2(0,0),
	float2(1,1),
	float2(1,0),
};


[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput>output
)
{
	
	float4 offset;
	GSOutput element;
	for (int i = 0; i < vnum; i++)
	{
		//offset = mul(matBillboard, offset_array[i]);
		offset = offset_array[i] * input[0].scale;
		offset = mul(matBillboard, offset);
		element.svpos = input[0].position + offset;
		//element.svpos = input[0].pos + offset_array[i];
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		element.color = input[0].color;
		output.Append(element);
	}
}