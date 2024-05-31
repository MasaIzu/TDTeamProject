#include "Tile.h"
#include"TextureManager.h"

Vector2 Tile::tileSpriteSize = { 64.0,64.0f };
ViewProjection Tile::lightViewProjection = {};
ViewProjection Tile::viewProjection = {};

void Tile::Initialize()
{
	//とりあえず白テクスチャ入れる
	sprite = Sprite::Create(TextureManager::Load("white1x1.png"));
	sprite->Initialize();
	sprite->SetSize(tileSpriteSize);
}

void Tile::Initialize(const std::string& modelFileName)
{
	//モデル生成
	model.reset( Model::CreateFromOBJ(modelFileName));
	
	//ワールド行列初期化
	world.Initialize();
}

void Tile::Update()
{
	//とりあえずZは固定
	world.translation_.z = 50.0f;
	//XYはスプライトで使ってたやつから持ってくる
	world.translation_.x = sprite->GetPosition().x;
	world.translation_.y = sprite->GetPosition().y;

	//ワールド行列更新
	world.TransferMatrix();
}

void Tile::DrawSprite()
{
	sprite->Draw(spritePos,spriteColor);
}

void Tile::DrawObject()
{
	model->Draw(world, Tile::viewProjection, Tile::lightViewProjection);
}
