#include "Tile.h"
#include"TextureManager.h"

Vector2 Tile::tileSpriteSize = { 64.0,64.0f };
ViewProjection Tile::lightViewProjection = {};
ViewProjection Tile::viewProjection = {};

void Tile::Initialize()
{
	//�Ƃ肠�������e�N�X�`�������
	sprite = Sprite::Create(TextureManager::Load("white1x1.png"));
	sprite->Initialize();
	sprite->SetSize(tileSpriteSize);
}

void Tile::Initialize(const std::string& modelFileName)
{
	//���f������
	model.reset( Model::CreateFromOBJ(modelFileName));
	
	//���[���h�s�񏉊���
	world.Initialize();
}

void Tile::Update()
{
	//�Ƃ肠����Z�͌Œ�
	world.translation_.z = 50.0f;
	//XY�̓X�v���C�g�Ŏg���Ă�����玝���Ă���
	world.translation_.x = sprite->GetPosition().x;
	world.translation_.y = sprite->GetPosition().y;

	//���[���h�s��X�V
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
