#pragma once
#include "Vector3.h"
#include "Vector4.h"


#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <DirectXMath.h>
MY_SUPPRESS_WARNINGS_END

/// <summary>
/// マトリックス
/// </summary>
class Matrix4
{
public:
	// Matrix4　構造体
	float m[4][4];

	// コンストラクタ
	Matrix4();

	//XMMatrixVer
	Matrix4(const DirectX::XMMATRIX& Matrix4);

	// 成分を指定しての生成
	Matrix4(
		const float& m00,const float& m01,const float& m02,const float& m03,
		const float& m10,const float& m11,const float& m12,const float& m13,
		const float& m20,const float& m21,const float& m22,const float& m23,
		const float& m30,const float& m31,const float& m32,const float& m33);




	// 単位行列を求める
	Matrix4 identity();

	//拡大縮小行列の設定
	Matrix4 scale(const Vector3& s);

	// 回転行列の設定
	Matrix4 rotateX(const float& angle);
	Matrix4 rotateY(const float& angle);
	Matrix4 rotateZ(const float& angle);

	// 平行移動行列の作成
	Matrix4 translate(const Vector3& t);

	// 座標変換（ベクトルと行列の掛け算をする）
	Vector3 transform(const Vector3& v, const Matrix4& m);


	//XMMATRIXに変換
	DirectX::XMMATRIX MyMatrixToXMMatrix(const Matrix4& Mat);

	//マトリックスの掛け算
	Matrix4 MatMul(const Matrix4& Mat);


	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m1);

	Matrix4 operator*(const Matrix4& m1);

private:

};

// 2項演算子オーバーロード

Vector3 operator*(const Vector3& v, const Matrix4& m);