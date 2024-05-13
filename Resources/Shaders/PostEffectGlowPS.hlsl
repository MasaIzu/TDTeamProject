#include"PostEffectGlow.hlsli"

Texture2D<float4> tex0 : register(t0);  	// 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);  	// 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      	// 0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{

	float offsetU = 1/ 1280.0f;
	float offsetV = 1 / 720.0f;

    tex1.Sample(smp, input.uv);
    
    if (shadeNumber == 0)
    {
        float4 col = tex0.Sample(smp, input.uv);
        //col += tex1.Sample(smp, input.uv);
        col.a = 1;

        return col;
    }
	else if (shadeNumber == 1) {
		float4 colortex0 = tex0.Sample(smp, input.uv);
		float4 colortex1 = tex1.Sample(smp, input.uv);

		//掛ける強度
		int kernelSize = KernelSize;

		// 平均値を計算するための総和
		float4 sum = float4(0, 0, 0, 0);

		// カーネルサイズの範囲内でピクセルをイテレート
		for (int y = -kernelSize; y <= kernelSize; ++y)
		{
			for (int x = -kernelSize; x <= kernelSize; ++x)
			{
				float2 offset = float2(x, y) * float2(offsetU, offsetV);
				float4 colortex0 = tex0.Sample(smp, input.uv + offset);
				sum += colortex0;
			}
		}

		// 総和をカーネルサイズで割ることで平均値を計算
		float kernelArea = (2 * kernelSize + 1) * (2 * kernelSize + 1);
		float4 averageColor = sum / kernelArea;

		float4 color = averageColor;

		//fmodってなに？ってなったから出す
		//x - y * floor(x / y);floorは浮動小数点値の端数を切り捨てて整数値に変換
		//uvは Xpix/画面横幅,Ypix/画面縦
		//if (fmod(input.uv.y, 0.1f) < 0.05f) {

		//	color = colortex1;
		//}
        
        float4 postColor = float4(color.rgb, 1);
        
        return postColor;
    }
	else if (shadeNumber == 2) {
		float2 uv = input.uv;
		float2 direction = uv - center;
		float2 step = direction / float(samples);

		float4 result = tex0.Sample(smp, uv);
		float totalWeight = 1.0;
		for (int i = 1; i < samples; ++i)
		{
			float weight = (float(samples) - float(i)) / float(samples);
			float2 sampleUV = uv - step * float(i) * intensity;
			result += tex0.Sample(smp, sampleUV) * weight;
			totalWeight += weight;
		}

		result /= totalWeight;
		return result;
	}
    else if (shadeNumber == 3)
    {
        float4 Color = tex0.Sample(smp, input.uv);
        float4 AddAllColor = tex0.Sample(smp, input.uv);
		
        float totalWeight = 0, _Sigma = 0.005, _StepWidth = 0.002; //Bloomはブラーを大げさに
        float4 col = float4(0, 0, 0, 0);
		
        for (float py = -_Sigma * 3; py <= _Sigma * 3; py += _StepWidth)//xyで2の幅で色を取得
        {
            for (float px = -_Sigma * 3; px <= _Sigma * 3; px += _StepWidth)
            {
                float2 pickUV = input.uv + float2(px, py);
				
                float4 colortex0 = tex0.Sample(smp, pickUV);
                float grayScale = colortex0.r + colortex0.g + colortex0.b;
                float extract = step(0.7, grayScale);
                float4 HighLumi = colortex0 * extract;
				
                float weight = Gaussian(input.uv, pickUV, _Sigma);
                col += HighLumi * weight;
				
                totalWeight += weight;
            }
        }
        col /= totalWeight;
		
        Color += col;
		
        Color.a = 1.0f;
		
        float sigma = 20.0; // ぼかしの強さを調整
        float pickRange = sigma * 3.0; // ガウシアン関数がほぼゼロになる範囲
        int steps = int(ceil(pickRange * 2.0)); // 計算するステップ数
        float2 texSize = float2(1280, 720); // 仮のテクスチャサイズ、実際のサイズに合わせてください

        float4 color = float4(0, 0, 0, 0);
        float totalWeight_ = 0.0;

        float minasWeight = 0.1f;
        
        for (int x = -steps; x <= steps; ++x)
        {
            float2 offset = float2(x, 0) / texSize;
            float2 pickUV = input.uv + offset;
        
            float4 colortex0 = tex1.Sample(smp, pickUV);
            float grayScale = (colortex0.r + colortex0.g + colortex0.b); // 輝度を計算
            float extract = step(2.5, grayScale); // しきい値0.1以上の輝度を持つピクセルを選択
            float4 HighLumi = colortex0 * extract; // 高輝度ピクセルの色

            float weight = Gaussian(input.uv, pickUV, sigma); // ガウシアン重み
        
            color += HighLumi * weight; // 重み付きカラーを加算
            totalWeight_ += weight; // 総重みを更新
        }

        color /= (totalWeight_ * minasWeight);
        
        Color += color;
		
        Color.a = 1.0f;
        
        return Color;
    }
    else if (shadeNumber == 4)
    {
        float4 AddAllColor = tex0.Sample(smp, input.uv);
		
        float totalWeight = 0;
        float totalWeight2 = 0;
        float4 color = float4(0, 0, 0, 0);
        float4 color2 = float4(0, 0, 0, 0);
        float2 pickUV = float2(0, 0);
        float pickRange = 0.06;
        float angleRad = _AngleDeg * 3.14159 / 180;
        float angleRad2 = _AngleDeg2 * 3.14159 / 180;
    
        float2 screen = float2(1280, 720);
			
		//ドット作るやつ
        //float2 st = input.uv / 1280 * 20;
        //st = frac(st * screen);
        //float l = distance(st, float2(0.5f, 0.5f));
        //float4 dot = float4(1, 1, 1, 1) * 1 - step(0.1, l);
		
        for (float i = -pickRange; i <= pickRange; i += 0.005)
        {
            float x = cos(angleRad) * i;
            float y = sin(angleRad) * i;
            pickUV = input.uv + float2(x, y);
			
            float4 colortex0 = tex0.Sample(smp, pickUV);
            float grayScale = colortex0.r + colortex0.g + colortex0.b;
            float extract = step(0.1, grayScale);
            float4 HighLumi = colortex0 * extract;
			
            float weight = Gaussian(input.uv, pickUV, pickRange);
			
            color += HighLumi * weight;
			
			
            totalWeight += weight;
        }
        for (float j = -pickRange; j <= pickRange; j += 0.005)
        {
            float x = cos(angleRad2) * j;
            float y = sin(angleRad2) * j;
            pickUV = input.uv + float2(x, y);
			
            float4 colortex0 = tex0.Sample(smp, pickUV);
            float grayScale = colortex0.r * 0.299 + colortex0.g * 0.587 + colortex0.b * 0.114;
            float extract = smoothstep(0.6, 0.9, grayScale);
            float4 HighLumi = colortex0 * extract;
			
            float weight = Gaussian(input.uv, pickUV, pickRange);
            color2 += HighLumi * weight;
			
			
            totalWeight2 += weight;
        }
		
        color /= totalWeight;
        color2 /= totalWeight2;
		
        AddAllColor += (color + color2);
		
        return AddAllColor;
    }
	
	return float4(1,1,1,1);
}