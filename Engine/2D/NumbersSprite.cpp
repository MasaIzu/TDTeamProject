#include "NumbersSprite.h"
#include <TextureManager.h>

NumbersSprite* NumbersSprite::NumbersSprite_ = nullptr;

NumbersSprite::~NumbersSprite()
{
	
}

uint32_t NumbersSprite::GetSpriteTexCount(const uint32_t& spriteNumber)
{
	return spriteTexNum[ spriteNumber ];
}

NumbersSprite* NumbersSprite::GetInstance()
{
	if ( NumbersSprite_ == nullptr )
	{
		NumbersSprite_ = new NumbersSprite();
	}

	return NumbersSprite_;
}

void NumbersSprite::Initialize()
{
	spriteTexNum[ 0 ] = TextureManager::Load("sprite/0.png");
	spriteTexNum[ 1 ] = TextureManager::Load("sprite/1.png");
	spriteTexNum[ 2 ] = TextureManager::Load("sprite/2.png");
	spriteTexNum[ 3 ] = TextureManager::Load("sprite/3.png");
	spriteTexNum[ 4 ] = TextureManager::Load("sprite/4.png");
	spriteTexNum[ 5 ] = TextureManager::Load("sprite/5.png");
	spriteTexNum[ 6 ] = TextureManager::Load("sprite/6.png");
	spriteTexNum[ 7 ] = TextureManager::Load("sprite/7.png");
	spriteTexNum[ 8 ] = TextureManager::Load("sprite/8.png");
	spriteTexNum[ 9 ] = TextureManager::Load("sprite/9.png");
}

void NumbersSprite::Destroy()
{
	delete NumbersSprite_;
}
