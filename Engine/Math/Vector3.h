#pragma once

/// <summary>
/// ベクター3
/// </summary>
class Vector3
{
public:
	float x;
	float y;
	float z;

public:
	//コンストラクタ
	Vector3();								//零ベクトルとする
	Vector3(const float& x,const float& y,const float& z);		//x成分,y成分,z成分 を指定して生成

	//メンバ関数
	float length() const;					//ノルム(長さ)を求める
	Vector3& normalize();					//正規化する
	Vector3 norm()const;
	float dot(const Vector3& v) const;		//内積を求める
	Vector3 cross(const Vector3& v) const;	//外積を求める

	Vector3 faceNormal(const Vector3& v1,const Vector3& v2,const Vector3& v3);

	//補間関数
	//線形補間(3次関数補間)
	static const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

	//単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	//代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator*=(const Vector3& v);
	Vector3& operator/=(float s);
	Vector3& operator=(float s[ 3 ]);

};


//　2項演算子オーバーロード
//　いろいろな引数のパターンに対応(引数の序列)するため、以下のように準備している
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator*(const Vector3& v1, const Vector3& v2);
const Vector3 operator/(const Vector3& v, float s);

