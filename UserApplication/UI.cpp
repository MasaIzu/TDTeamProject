#include "UI.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "Easing.h"

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
	// PlayerHP Initialize
	{
		playerHPBAR_ = Sprite::Create(TextureManager::Load("sprite/white1x1.png"));
		playerHPBARPos_.x = WinApp::GetInstance()->GetWindowSize().x / 2;
		playerHPBARPos_.y = 650.0f;
		downPLHp_ = false;
		playerOldHP_ = 20;
	}
	timeSprite_ = Sprite::Create(TextureManager::Load("sprite/Green30.png"));
}

void UI::Update()
{
	PlayerHPUpdate();

	timeSprite_->SetSize({ ( float ) timeGauge_,50.0f });
}

void UI::Draw()
{
	playerHPBAR_->Draw(playerHPBARPos_,playerHPColor_);
	timeSprite_->Draw({ WinApp::GetInstance()->GetWindowSize().x / 2,100 },{ 1,1,1,1 },1);
}

float UI::lerpFloat(const float start,const float end,const float t) {
	return start * ( 1.0f - t ) + end * t;
}

void UI::PlayerHPUpdate()
{
	if ( playerOldHP_ != playerHP_ )
	{
		downPLHp_ = true;
	}
	else
	{
		downPLHp_ = false;
	}

	playerHPBAR_->SetSize({ lerpFloat(0.0f,300.0,( float ) playerHP_ / ( float ) playerStartHP_),20.0f });
	if ( downPLHp_ )
	{
		playerHPColor_ = { 0.8f,0,0,1 };
	}
	else
	{
		playerHPColor_ = { 0.8f,0.8f,0.8f,1 };
	}
	playerOldHP_ = playerHP_;
}
