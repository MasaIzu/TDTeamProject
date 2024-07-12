#include "UI.h"
#include "TextureManager.h"

UI::UI()
{

}

UI::~UI()
{
}

UI* UI::Get_Instans()
{
	static UI instance;
	return &instance;
}

void UI::Initialize()
{
	SpriteTest_ = Sprite::Create(TextureManager::Load("sprite/doragon.png"));

}

void UI::Update()
{


}

void UI::Draw()
{
	SpriteTest_->Draw({ 100,100 },{ 1,1,1,1 });

}
