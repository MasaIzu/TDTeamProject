//タイルクラス
//ランダム生成されるうちの1つずつのマップ

#pragma once
#include"Sprite.h"
class Tile
{
public:

	static Vector2 tileSpriteSize;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//setter
	void SetSpritePos(const Vector2& pos) { spritePos = pos; }
	void SetSpriteColor(const Vector4& color) { spriteColor = color; }

	//getter
	const Vector2& GetSpritePos() const { return spritePos; }
	const Vector4& GetSpriteColor() const { return spriteColor; }

private:

	

	std::unique_ptr<Sprite> sprite = nullptr;

	Vector2 spritePos = { 0,0 };

	Vector4 spriteColor = { 1,1,1,1 };


};

