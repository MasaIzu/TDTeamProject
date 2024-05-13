#pragma once
#include "Sprite.h"
#include "NumbersSprite.h"
#include <vector>

class SerialNumber
{

public://基本関数
	SerialNumber( );
	~SerialNumber( );

	//初期化
	void Initialize(const uint32_t& makeCount);
	//更新
	void Update(const float& nowNumber);
	//描画
	void Draw();

private://メンバ関数

	//判別関数
	void Discriminant(const float& nowNumber);

	//数字返却
	uint32_t ReturnNumber(const float& nowNumber,const uint32_t& place);

public://Setter
	void SetAllContent(const float& TerritorySize,const Vector2& referencePointPos,const Vector2& spriteSize);
	void SetTerritory(const float& TerritorySize);
	void SetReferencePointPos(const Vector2& referencePointPos);
	void SetSpriteSize(const Vector2& spriteSize);
public://Getter

private://コンスト
	static const uint32_t SpriteNumberCount = 10;
	
private://クラス関連
	std::vector<std::vector<std::unique_ptr<Sprite>>> HpSprite;
private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数

	bool isChange = false;

	uint32_t One = 0;
	uint32_t Ten = 0;
	uint32_t Hundred = 0;
	uint32_t Thousand = 0;
	uint32_t DiscriminantCount = 0;

	float Territory = 25.0f;
	float ThousandNumber = 1000.0f;
	float HundredNumber = 100.0f;
	float TenNumber = 10.0f;

	Vector2 ReferencePointPos = { 640,320 };
	Vector2 SpriteSize = { 28,27 };
	Vector2 FirstPos;

	Vector4 White = { 1,1,1,1 };
	Vector4 Red = { 1,0,0,1 };

	std::vector <uint32_t> Number;
	std::vector <Vector2> SpritePos;
};