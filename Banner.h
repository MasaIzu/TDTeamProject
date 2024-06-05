#pragma once
#include "Sprite.h"
#include "Vector2.h"
#include "Vector4.h"

class Banner
{
public:
	void Initialize();
	void Update();
	void Draw();

	bool GetAnimEnd() { return isAnimEnd; };
private:
	std::unique_ptr<Sprite> start_;
	Vector2 startPos_;
	Vector4 color_;

	// バナーイージング用
	float bannerEasetime = 0.0f;
	int32_t bannerTime = 0;
	int32_t bannerEaseMaxTime = 50;
	bool isAnimEnd;
	bool isBannerEaseOut;
};

