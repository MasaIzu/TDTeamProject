#include "Banner.h"
#include "TextureManager.h"
#include "Input.h"
#include "Easing.h"

void Banner::Initialize()
{
	start_ = Sprite::Create(TextureManager::Load("sprite/START2.png"));
	startPos_ = { 100,100 };
	color_ = { 1,1,1,1 };
	isBannerEaseOut = false;
	isAnimEnd = false;
}

void Banner::Update()
{
	Vector2 move = { 0,0 };
	Vector3 easepos = { 0,0,0 };
	//if(Input::GetInstance()->PushKey(DIK_L) ){
	//	move.x = 1;
	//}
	//if (Input::GetInstance()->PushKey(DIK_J)) {
	//	move.x = -1;
	//}
	//if (Input::GetInstance()->PushKey(DIK_I)) {
	//	move.y = -1;
	//}
	//if (Input::GetInstance()->PushKey(DIK_K)) {
	//	move.y = 1;
	//}
	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		startPos_ = { -100,720 / 2 };
		bannerTime = 0;
		isBannerEaseOut = false;
		isAnimEnd = false;
	}
	if (isAnimEnd == false) {
		bannerTime++;
	}
	if (bannerTime >= bannerEaseMaxTime && isBannerEaseOut == false) {
		bannerTime = 0;
		isBannerEaseOut = true;
	}
	else if (bannerTime >= bannerEaseMaxTime && isBannerEaseOut == true) {
		isAnimEnd = true;
	}

	if (isBannerEaseOut == false) {
		easepos = Easing::EaseOutQuintVec3({ -100,720 / 3,0 }, { 1280 / 2,720 / 3,0 }, bannerTime, bannerEaseMaxTime);
	}
	else {
		easepos.x = Easing::EaseInQuint(1280 / 2, 1280 + 100, (float)bannerTime, (float)bannerEaseMaxTime);
		easepos.y = Easing::EaseInQuint(720 / 3, 720 / 3, (float)bannerTime, (float)bannerEaseMaxTime);
	}

	move.x = easepos.x;
	move.y = easepos.y;
	startPos_ = move;
}

void Banner::Draw()
{
	if (isAnimEnd == false) {
		start_->Draw(startPos_, color_, 1);
	}
}
