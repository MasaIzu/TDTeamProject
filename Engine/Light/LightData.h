#pragma once
#include <vector>
#include "LightGroup.h"
#include <memory>
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

class LightData
{

public://基本関数

	// シングルトンインスタンスの取得
	static LightData* GetInstance();
	// 終了処理
	void Destroy();
	// シーン終了時処理
	void ClearLight();

	//初期化
	void StaticInitialize();
	//更新
	void Update();

private:
	LightData() = default;
	~LightData();
	LightData(const LightData&) = delete;
	const LightData& operator=(const LightData&) = delete;

	static LightData* LightData_;
	static LightGroup* lightGroup;

private://メンバ関数

public://Setter

	/// <summary>
	/// ポイントライト
	/// </summary>
	/// <param name="lightpos_">ポジション</param>
	/// <param name="lightcolor_">カラー</param>
	/// <param name="lightAtten_">減衰係数</param>
	/// <param name="active_">フラグ</param>
	/// <returns></returns>
	uint32_t AddPointLight(const Vector3& lightpos_,const Vector3& lightcolor_,const Vector3& lightAtten_,const bool& active_ = true);
	/// <summary>
	/// スポットライト
	/// </summary>
	/// <param name="lightpos_">ポジション</param>
	/// <param name="lightdir_">ライトの向いている方向</param>
	/// <param name="lightcolor_">色</param>
	/// <param name="lightAtten_">減衰係数</param>
	/// <param name="active_">フラグ</param>
	/// <returns></returns>
	uint32_t AddSpotLight(const Vector3& lightpos_,const Vector4& lightdir_,const Vector3& lightcolor_,const Vector3& lightAtten_,const bool& active_ = true);
	/// <summary>
	/// 丸影
	/// </summary>
	/// <param name="casterPos_">キャスターポジション</param>
	/// <param name="dir_">ライトの距離</param>
	/// <param name="atten_">距離減衰係数</param>
	/// <param name="factorAngle_">減衰角度</param>
	/// <param name="active_">フラグ</param>
	/// <returns></returns>
	uint32_t AddCircleShadow(const Vector3& casterPos_,const float& distanceCasterLight,const Vector4& dir_,const Vector3& atten_,const Vector2& factorAngle_,const bool& active_ = true);

	/// <summary>
	/// ポイントライト
	/// </summary>
	/// <param name="LightNumbar">ライトのナンバー</param>
	/// <param name="lightpos_">ポジション</param>
	/// <param name="lightcolor_">カラー</param>
	/// <param name="lightAtten_">減衰係数</param>
	/// <param name="active_">フラグ</param>
	/// <returns></returns>
	void UpdatePointLight(const uint32_t& LightNumbar,const Vector3& lightpos_,const Vector3& lightcolor_,const Vector3& lightAtten_,const bool& active_ = true);

	/// <summary>
	/// スポットライト
	/// </summary>
	/// <param name="lightpos_">ポジション</param>
	/// <param name="lightdir_">ライトの向いている方向</param>
	/// <param name="lightcolor_">色</param>
	/// <param name="lightAtten_">減衰係数</param>
	/// <param name="active_">フラグ</param>
	/// <returns></returns>
	void UpdateSpotLight(const uint32_t& LightNumbar,const Vector3& lightpos_,const Vector4& lightdir_,const Vector3& lightcolor_,const Vector3& lightAtten_,const bool& active_ = true);

	/// <summary>
	/// 丸影
	/// </summary>
	/// <param name="casterPos_">キャスターポジション</param>
	/// <param name="dir_">ライトの距離</param>
	/// <param name="atten_">距離減衰係数</param>
	/// <param name="factorAngle_">減衰角度</param>
	/// <param name="active_">フラグ</param>
	/// <returns></returns>
	void UpdateCircleShadow(const uint32_t& ShadowNumbar,const Vector3& casterPos_,const float& distanceCasterLight,const Vector4& dir_,const Vector3& atten_,const Vector2& factorAngle_,const bool& active_ = true);

	void SetAmbientColor(const float& AmbientColor);

public://Getter
	LightGroup* GetLightGroupData();

private://クラス関連
	

private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数
	
	

};