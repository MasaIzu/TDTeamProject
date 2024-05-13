#pragma once

/// <summary>
/// ベクター4
/// </summary>
class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;
public:
	//コンストラクタ
	Vector4();								//零ベクトルとする
	Vector4(const float& x,const float& y,const float& z,const float& w);		//x成分,y成分,z成分 を指定して生成

	//メンバ関数
	float length() const;					//ノルム(長さ)を求める
	Vector4& normalize();					//正規化する
	float dot(const Vector4& v) const;		//内積を求める
	Vector4 cross(const Vector4& v) const;	//外積を求める


	float Vector3Length()const;//ノルムを求める
	float Vector3LengthSquared()const;
	Vector4 Vector3Normalization()const;//正規化する
	Vector4& Vector3Normal();//正規化する
	float Vector3Dot(const Vector4& v)const;//内積を求める
	Vector4 Vector3Cross(const Vector4& v)const;//外積を求める


	//単項演算子オーバーロード
	Vector4 operator+() const;
	Vector4 operator-() const;

	//代入演算子オーバーロード
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s);
	Vector4& operator=(float v[ 4 ]);

};


//　2項演算子オーバーロード
//　いろいろな引数のパターンに対応(引数の序列)するため、以下のように準備している
const Vector4 operator+(const Vector4& v1, const Vector4& v2);
const Vector4 operator-(const Vector4& v1, const Vector4& v2);
const Vector4 operator*(const Vector4& v, float s);
const Vector4 operator*(float s, const Vector4& v);
const Vector4 operator/(const Vector4& v, float s);

void Vector4Lerp(const Vector4& src1, const Vector4& src2, float t, Vector4& dest);

Vector4 Vector4Lerp(const Vector4& src1, const Vector4& src2, float t);