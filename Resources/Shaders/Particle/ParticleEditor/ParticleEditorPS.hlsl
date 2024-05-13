#include "ParticleEditor.hlsli"


Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

struct PSoutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};

PSoutput main(GSOutput input) : SV_TARGET
{
	//float3 light = normalize(float3(1,-1,1)); // 右下奥　向きのライト
	//float diffuse = saturate(dot(-light, input.normal));
	//float brightness = diffuse + 0.3f;
	//float4 texcolor = tex.Sample(smp, input.uv) * color;
	//return tex.Sample(smp,input.uv)*input.color;
	//return float4(0, 0, 0, 0);
	//float4 texcolor = tex.Sample(smp, input.uv) * input.color;
    PSoutput output;
	
    output.target0 = tex.Sample(smp, input.uv) * input.color;
    output.target1 = tex.Sample(smp, input.uv) * input.PostEffectPowColor;
	
    return output;
}