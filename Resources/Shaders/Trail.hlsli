
cbuffer ViewProjection : register(b0)
{
    matrix view; // ビュー変換行列
    matrix projection; // プロジェクション変換行列
    float3 cameraPos; // カメラ座標（ワールド座標）
};

cbuffer ConstantBufferColor : register(b1)
{
    float4 colorA;
};

//頂点シェーダの出力構造体
//（頂点シェーダーからピクセルシェーダーへのやり取りに使用する）
struct VSOutput {
	//システム用頂点座標
	float4 svpos : SV_POSITION;
	//uv座標
	float2 uv :TEXCOORD;
    //color
    float4 color : TEXCOLOR;
};