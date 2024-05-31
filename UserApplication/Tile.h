//タイルクラス
//ランダム生成されるうちの1つずつのマップ

#pragma once
#include"Sprite.h"
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"

class Tile
{
public:
	/// <summary>
	/// 静的メンバ
	/// </summary>

	static Vector2 tileSpriteSize;
	static ViewProjection viewProjection;
	static ViewProjection lightViewProjection;

	/// <summary>
	/// ビュー行列のセット
	/// </summary>
	/// <param name="view"></param>
	static void SetViewProjection(const ViewProjection& view) { Tile::viewProjection = view; }

	/// <summary>
	/// ライトのビュー行列のセット
	/// </summary>
	/// <param name="lightView"></param>
	static void SetLightView(const ViewProjection& lightView) { Tile::lightViewProjection = lightView; }

public:
	///動的メンバ


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// モデル読みつき初期化
	/// </summary>
	/// <param name="modelFileName"></param>
	void Initialize(const std::string& modelFileName);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画(スプライト)
	/// </summary>
	void DrawSprite();

	/// <summary>
	/// 描画(オブジェクト)
	/// </summary>
	void DrawObject();

	//setter
	void SetSpritePos(const Vector2& pos) { spritePos = pos; }
	void SetSpriteColor(const Vector4& color) { spriteColor = color; }

	//getter
	const Vector2& GetSpritePos() const { return spritePos; }
	const Vector4& GetSpriteColor() const { return spriteColor; }

private:

	

	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Model> model = nullptr;

	WorldTransform world;
	Vector2 spritePos = { 0,0 };
	Vector4 spriteColor = { 1,1,1,1 };


};

