#pragma once
#include "Sprite.h"

class NumbersSprite
{

public://基本関数
	static NumbersSprite* GetInstance();

	//初期化
	void Initialize();

	//消す
	void Destroy();

private://メンバ関数

private:
	NumbersSprite() = default;
	~NumbersSprite();
	NumbersSprite(const NumbersSprite&) = delete;
	const NumbersSprite& operator=(const NumbersSprite&) = delete;

public://Setter

public://Getter
	uint32_t GetSpriteTexCount(const uint32_t& spriteNumber);

private://コンスト
	static const uint32_t spriteCount = 10;

private://クラス関連

private:// 静的メンバ変数

	static NumbersSprite* NumbersSprite_;

private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数

	uint32_t spriteTexNum[ spriteCount ];

};