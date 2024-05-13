#include "Obj.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> shadowMap : register(t1); //1番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー


float4 main(GSOutput input) : SV_TARGET
{
	// テクスチャマッピング
    float4 texcolor = tex.Sample(smp, input.uv);
		
	// 光沢度
    const float shininess = 4.0f;
	// 頂点から視点への方向ベクトル
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);


	// シェーディングによる色
    float4 shadecolor = float4(ambientColor, m_alpha);


	//点光源
    for (int i = 0; i < POINTLIGHT_MAXNUM; i++)
    {
        if (pointLights[i].active)
        {

			//ライトへのベクトル
            float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			//ベクトルの長さ
            float d = length(lightv);
			//正規化し、単位ベクトルとする
            lightv = normalize(lightv);

			//距離減衰係数
            float atten = 3.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);

			//ライトに向かうベクトルと法線のない席
            float3 dotlightnormal = dot(lightv, input.normal);
			//反射光ベクトル
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
            float3 diffuse = dotlightnormal;
            //float3 diffuse = (0,0,0);
			//鏡面反射光
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess);
            //float3 specular = (0, 0, 0);
			//すべて加算する
            shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;

        }
    }

	// スポットライト
    for (i = 0; i < spotLightCount; i++)
    {
        if (spotLights[i].active)
        {
			// ライトへの方向ベクトル
            float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
            float d = length(lightv);
            lightv = normalize(lightv);

			// 距離減衰係数
            float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));

			// 角度減衰
            float cos = dot(lightv, spotLights[i].lightv);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
            float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
			// 角度減衰を乗算
            atten *= angleatten;

			// ライトに向かうベクトルと法線の内積
            float3 dotlightnormal = dot(lightv, input.normal);
			// 反射光ベクトル
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
            float3 diffuse = dotlightnormal;
			// 鏡面反射光
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess);

			// 全て加算する
            shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
        }
    }

	// 丸影
    for (i = 0; i < circleShadowCount; i++)
    {
        if (circleShadows[i].active)
        {
			// オブジェクト表面からキャスターへのベクトル
            float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			// 光線方向での距離
            float d = dot(casterv, circleShadows[i].dir);

			// 距離減衰係数
            float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
			// 距離がマイナスなら0にする
            atten *= step(0, d);

			// ライトの座標
            float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//  オブジェクト表面からライトへのベクトル（単位ベクトル）
            float3 lightv = normalize(lightpos - input.worldpos.xyz);
			// 角度減衰
            float cos = dot(lightv, circleShadows[i].dir);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
            float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			// 角度減衰を乗算
            atten *= angleatten;

			// 全て減算する
            shadecolor.rgb -= atten;
        }
    }

	//影(シャドウマップ)
    float shadow = 1.0f;
   
    if (shadowMapFlag)
    {
        //シャドウマップのZ値を参照
        float w = 1.0f / input.shadowpos.w;
        float2 shadowTexUV;
        shadowTexUV.x = (1.0f + input.shadowpos.x * w) * 0.5f;
        shadowTexUV.y = (1.0f - input.shadowpos.y * w) * 0.5f;
    
        float4 map = shadowMap.Sample(smp, shadowTexUV);
        float inputZ = input.shadowpos.z * w;
    
	    //uv座標で0～1なら影判定をする
        if (shadowTexUV.x >= 0 && shadowTexUV.x <= 1.0f && shadowTexUV.y >= 0 && shadowTexUV.y <= 1.0f)
        {
            if (map.x + 0.0000007f < inputZ)
            {
                shadow *= 0.5f;
            }
        }
    }

    

	// シェーディングによる色で描画;
    return shadecolor * texcolor * float4(shadow, shadow, shadow, 1);
}