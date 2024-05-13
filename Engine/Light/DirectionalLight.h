#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

class DirectionalLight {
private: //静的メンバ変数
//デバイス
	static ID3D12Device* device;

public://サブクラス　
	//定数バッファ用データ構造
	struct ConstBufferData
	{
		Vector4 lightv; //ライトへの方向を表すベクトル
		Vector3 lightcolor; //ライトの色
		unsigned int active;
	};

public:

	/// <summary>
	/// ライト方向をセット
	/// </summary>
	void SetLightDir(const Vector4& lightdir_)
	{
		//正規化してセット
		this->lightdir = lightdir_.Vector3Normalization();
	}

	/// <summary>
	/// ライト色をセット
	/// </summary>
	void SetLightColor(const Vector3& lightcolor_)
	{
		this->lightcolor = lightcolor_;
	}

	/// <summary>
	/// 有効フラグをセット
	/// </summary>
	/// <param name="active">有効フラグ</param>
	inline void SetActive(bool active_) { this->active = active_; }

	
	/// <summary>
	/// 有効チェック
	/// </summary>
	/// <returns>有効フラグ</returns>
	inline bool IsActive() { return active; }

	Vector4 GetLightDir() { return lightdir; }
	Vector3 GetLightColor() { return lightcolor; }

private://メンバ変数
	//ライト光線方向
	Vector4 lightdir = { 1,0,0,0 };
	//ライト色
	Vector3 lightcolor = { 1,1,1 };
	//ダーティフラグ
	bool dirty = false;

	//有効フラグ
	bool active = true;
};

