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
	{
		playerHPBAR_ = Sprite::Create(TextureManager::Load("sprite/white1x1.png"));
		playerHPBARPos_.x = WinApp::GetInstance()->GetWindowSize().x / 2;
		playerHPBARPos_.y = 650.0f;
		downPLHp_ = false;
		playerOldHP_ = 20;
	}
	timeSprite_ = Sprite::Create(TextureManager::Load("sprite/white1x1.png"));

	easeTimer_ = 0;
	easeMaxTime_ = 50;
	easeHalfOver_ = false;

	timeColor_ = { 0,1,0,1 };
}

void UI::Update(const bool& startBannerEnd)
{
	if ( startBannerEnd == true )
	{
		PlayerHPUpdate();
		TimeUpdate();
	}
	else
	{
		float guageSize = 0;
		drawStart_ = false;
		easeTimer_++;
		if ( easeHalfOver_ )
		{
			guageSize = Easing::EaseInQuint(0,timeRest_,( float ) easeTimer_,( float ) easeMaxTime_);
		}
		timeSprite_->SetSize({ guageSize,50.0f });
		if ( easeTimer_ >= 50 && easeHalfOver_ == false )
		{
			easeHalfOver_ = true;
			easeTimer_ = 0;
		}
		else if ( easeTimer_ >= 50 && easeHalfOver_ == true )
		{
			drawStart_ = true;
			easeTimer_ = 50;
		}
	}

}

void UI::Draw()
{
	if ( drawStart_ )
	{
		playerHPBAR_->Draw(playerHPBARPos_,playerHPColor_);
	}
	timeSprite_->Draw({ WinApp::GetInstance()->GetWindowSize().x / 2,100 },timeColor_);
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

void UI::TimeUpdate()
{
	timeSprite_->SetSize({ ( float ) timeGauge_,50.0f });
	switch ( nowPhase_ )
	{
	case 1:
		timeColor_ = { 0,1,0,1 };
		break;
	case 2:
		timeColor_ = { 1,0.5f,0,1 };
		break;
	case 3:
		timeColor_ = { 1,0,0,1 };
		break;
	default:
		break;
	}

}
