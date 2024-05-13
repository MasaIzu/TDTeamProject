#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "MyMath.h"

#include "Quaternion.h"

#pragma warning(push)
#pragma warning(disable: 4820)
#include <d3d12.h>
#include <wrl.h>
#pragma warning(pop)

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld;           // ローカル → ワールド変換行列
	float alpha=1;       // アルファ
	uint32_t shadowMapFlag = 1;
};

//その物体の向いている方向
struct WorldTarnsLook {

	Vector3 look = { 0,0,0 };
	Vector3 lookBack = { 0,0,0 };
	Vector3 lookRight = { 0,0,0 };
	Vector3 lookLeft = { 0,0,0 };

	Vector3 lookUp = { 0,0,0 };
	Vector3 lookDown = { 0,0,0 };

	Vector3 look_lookRight = { 0,0,0 };
	Vector3 look_lookLeft = { 0,0,0 };
	Vector3 lookBack_lookRight = { 0,0,0 };
	Vector3 lookBack_lookLeft = { 0,0,0 };

	Vector3 lookUp_look = { 0,0,0 };
	Vector3 lookDown_look = { 0,0,0 };

	Vector3 lookUp_lookLeft = { 0,0,0 };
	Vector3 lookUp_lookRight = { 0,0,0 };
	Vector3 lookDown_lookLeft = { 0,0,0 };
	Vector3 lookDown_lookRight = { 0,0,0 };

};

/// <summary>
/// ワールドデータ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピングアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale_ = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0, 0, 0 };
	// ローカル座標
	Vector3 translation_ = { 0, 0, 0 };
	//任意軸のローカル回転
	Quaternion quaternion = { 0,0,0,0 };
	// ローカル → ワールド変換行列
	Matrix4 matWorld_;
	//アルファ値
	float alpha = 1;
	//アルファ値
	uint32_t shadowMapFlag = 1;

	//その物体の向いている方向
	WorldTarnsLook LookVelocity;

	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;
	//回転用Matrix
	Matrix4 matRot;

	//回転がオイラー角による回転か
	bool isEuler = false;

	//回転が向いている方向のみ
	bool IsLookOnlyMatRot = false;

	//初期化
	void Initialize();
	//定数バッファ生成
	void CreateConstBuffer();
	//マッピングする
	void Map();
	//行列を転送する
	void TransferMatrix();

	/// <summary>
	/// 回転セット
	/// </summary>
	/// <param name="rot"></param>
	void SetRot(const Vector3& rot);

	/// <summary>
	/// Matrix回転セット
	/// </summary>
	void SetMatRot(const Matrix4& mat);

	/// <summary>
	/// 回転をプラスしていく
	/// </summary>
	/// <param name="move"></param>
	void MoveRot(const Vector3& move);

	/// <summary>
	/// クオータニオンロット
	/// </summary>
	/// <param name="quater"></param>
	void SetQuater(const Quaternion& quater);

	/// <summary>
	/// クオータニオンプラスしていく
	/// </summary>
	/// <param name="move"></param>
	void MoveQuater(const Quaternion& move);

	/// <summary>
	/// クオータニオンゲット
	/// </summary>
	Quaternion& GetQuaternion();

	/// <summary>
	/// 見てる方向のゲット
	/// </summary>
	Vector3 GetLook(const Matrix4& matRot,const Vector3& at);

	/// <summary>
	/// 見る方向のセット
	/// </summary>
	void SetLookMatRot(const Matrix4& mat);

private:
	Matrix4 worldLookMatRot;
};