#pragma once

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <cstdint>
MY_SUPPRESS_WARNINGS_END

/// <summary>
/// uint32_tのベクター
/// </summary>
class Uint32Vector2
{
public:
	uint32_t x;//x成分
	uint32_t y;//y成分

public:
	//コンストラクタ
	Uint32Vector2();							//零ベクトルとして生成
	Uint32Vector2(const uint32_t& x,const uint32_t& y);			//x成分,y成分を指定して生成


	//代入演算子オーバーロード
	Uint32Vector2& operator+=(const Uint32Vector2& v);
	Uint32Vector2& operator-=(const Uint32Vector2& v);
	Uint32Vector2& operator*=(uint32_t s);
	Uint32Vector2& operator/=(uint32_t s);

};


//2項目演算子オーバーロード
//いろいろな引数(引数の型と序列)のパターンに対応するため、以下のようにしている

const Uint32Vector2 operator+(const Uint32Vector2& v1, const Uint32Vector2& v2);
const Uint32Vector2 operator-(const Uint32Vector2& v1, const Uint32Vector2& v2);
const Uint32Vector2 operator*(const Uint32Vector2& v, uint32_t s);
const Uint32Vector2 operator*(uint32_t s, const Uint32Vector2& v);
const Uint32Vector2 operator/(const Uint32Vector2& v, uint32_t s);