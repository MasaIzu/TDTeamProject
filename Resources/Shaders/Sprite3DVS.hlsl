#include "Sprite3D.hlsli"


VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output; // ピクセルシェーダーに渡す値
    output.svpos = mul(mul(mul(projection, view), mat), pos);
    output.uv = uv;
    return output;
}