#include "Tile.h"
#include"TextureManager.h"

Vector2 Tile::tileSpriteSize = { 64.0,64.0f };

void Tile::Initialize()
{
	//�Ƃ肠�������e�N�X�`�������
	sprite = Sprite::Create(TextureManager::Load("white1x1.png"));
	sprite->Initialize();
	sprite->SetSize(tileSpriteSize);
}

void Tile::Update()
{
}

void Tile::Draw()
{
	sprite->Draw(spritePos,spriteColor);
}
