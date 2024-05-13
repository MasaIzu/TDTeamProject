#pragma once
#include"ErrorException.h"
#include"Vector4.h"
#include"Vector3.h"
#include"Matrix4.h"

/// <summary>
/// クオータニオン
/// </summary>
class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	//クオータニオンを作成

	//成分を指定して クォータニオンを作成
	Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);

	//任意軸まわりの回転の指定からクォータニオンを作成
	Quaternion(const Vector3& v, float angle);

	//回転行列からクォータニオンを作成
	Quaternion(const Matrix4& m);

	//Vector4からクォータニオンを作成
	Quaternion(const Vector4& v);

	//ベクトルからベクトルの角度
	Quaternion(const Vector3& u, const Vector3& v);

	//内積を求める
	float Dot(const Quaternion& q)const;

	//ノルムを求める
	float Norm()const;

	//正規化する
	Quaternion Normalize() const;

	//単位クオータニオン
	Quaternion Identity() const;

	// 共役クオータニオン
	Quaternion Conjugate() const;

	// 逆クオータニオン
	Quaternion Inverse() const;

	//クオータニオン球面線形補間
	Quaternion Slerp(const Quaternion& q, float t);

	//クオータニオン線形補間
	Quaternion Lerp(const Quaternion& q, float t);

	//クオータニオンから回転行列を求める
	Matrix4 Rotate();

	//回転軸の算出
	Vector3 GetAxis();

	//オイラー角に変換
	Vector3 GetEulerAngles();

	//オイラー角からクオータニオンに変換
	void SeTEuler(const Vector3& rot);

	//単項演算子オーバーロード
	Quaternion operator + ();
	Quaternion operator - ()const;

	//代入演算子オーバーロード
	Quaternion& operator += (const Quaternion& q);
	Quaternion& operator -= (const Quaternion& q);
	Quaternion& operator *= (float s);
	Quaternion& operator /= (float s);
	Quaternion& operator *= (const Quaternion& q);

	//ベクター3を掛け算
	Vector3 multiply(const Vector3& vec);

};

//2項演算子オーバーロード
const Quaternion operator + (const Quaternion& q1, const Quaternion& q2);
const Quaternion operator - (const Quaternion& q1, const Quaternion& q2);
const Quaternion operator * (const Quaternion& q1, const Quaternion& q2);
const Quaternion operator * (const Quaternion& q, float s);
const Quaternion operator * (float s, const Quaternion& q);
const Quaternion operator / (const Quaternion& q, float s);
const Vector3 operator*(Quaternion q, Vector3 vec);

