cbuffer WorldTransform : register(b0) {
	matrix world; // ワールド行列
	float m_alpha;	// アルファ
    uint shadowMapFlag; //シャドウ用
};


cbuffer ViewProjection : register(b1) {
	matrix view;       // ビュー変換行列
	matrix projection; // プロジェクション変換行列
	float3 cameraPos; // カメラ座標（ワールド座標）
};

cbuffer LightViewProjection : register(b2)
{
    matrix LightView; // ビュー変換行列
    matrix LightProjection; // プロジェクション変換行列
    float3 LightCameraPos; // カメラ座標（ワールド座標）
};

cbuffer Material : register(b3) {
	float3 m_ambient  : packoffset(c0); // アンビエント係数
	float3 m_diffuse  : packoffset(c1); // ディフューズ係数
	float3 m_specular : packoffset(c2); // スペキュラー係数
}

// 点光源の数
static const int POINTLIGHT_MAXNUM = 30;

struct PointLight
{
	float3 lightpos;    // ライト座標
	float3 lightcolor;  // ライトの色(RGB)
	float3 lightatten;	// ライト距離減衰係数
	uint active;
};

// スポットライトの数
static const int SPOTLIGHT_MAXNUM = 30;

struct SpotLight
{
	float3 lightv;		// ライトの光線方向の逆ベクトル（単位ベクトル）
	float3 lightpos;    // ライト座標
	float3 lightcolor;  // ライトの色(RGB)
	float3 lightatten;	// ライト距離減衰係数
	float2 lightfactoranglecos; // ライト減衰角度のコサイン
	uint active;
};

// 丸影の数
static const int CIRCLESHADOW_MAXNUM = 3;

struct CircleShadow
{
	float3 dir;		// 投影方向の逆ベクトル（単位ベクトル）
	float3 casterPos;    // キャスター座標
	float  distanceCasterLight;	// キャスターとライトの距離
	float3 atten;	// 距離減衰係数
	float2 factorAngleCos; // 減衰角度のコサイン
	uint active;
};

cbuffer LightGroup : register(b4)
{
	float3 ambientColor;
    uint pad;
	
    uint pointLightCount;
    uint spotLightCount;
    uint circleShadowCount;
    float pad2;
	
    PointLight pointLights[POINTLIGHT_MAXNUM];
    SpotLight spotLights[SPOTLIGHT_MAXNUM];
    CircleShadow circleShadows[CIRCLESHADOW_MAXNUM];
}

cbuffer PolygonExplosion: register(b5)
{
	float _Destruction : packoffset(c0);
	float _ScaleFactor : packoffset(c0.y);
	float _RotationFactor : packoffset(c0.z);
	float _PositionFactor : packoffset(c0.w);
}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float4 worldpos : POSITION; // ワールド座標
	float3 normal :NORMAL; // 法線
	float2 uv  :TEXCOORD; // uv値
    float4 shadowpos : POSITION1;
};

struct GSOutput
{
	//システム用頂点座標
	float4 svpos : SV_POSITION;
	float4 worldpos : POSITION; // ワールド座標
	float3 normal:NORMAL;//法線ベクトル
	float2 uv:TEXCOORD;//uv値
    float4 shadowpos : POSITION1;
};