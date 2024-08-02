#pragma once
#include "Sprite.h"

class Player;
class Enemy;

class UI
{
public:
	UI();
	~UI();
	static UI* Get_Instans();

	void Initialize();
	void Update(const bool& startBannerEnd);
	void Draw();

	//void SetPlayerPointer(Player* player) {	player_ = player;}
	void SetPlayerHP(const int32_t playerHP) {
		playerHP_ = playerHP;
	}
	void SetPlayerStartHP(const int32_t playerHP) {
		playerStartHP_ = playerHP;
	}
	void SetTimeGauge(const int32_t& timeGauge) {
		timeGauge_ = timeGauge;
	}
	void SetTimeRest(const int32_t& timeRest) {
		timeRest_ = timeRest;
	}
	void SetNowPhase(const int32_t& nowPhase) {
		nowPhase_ = nowPhase;
	}
	float lerpFloat(const float start,const float end,const float t);

private:
	void PlayerHPUpdate();	// player関連
	void TimeUpdate();
public:


private:


	std::unique_ptr<Sprite> playerHPBAR_ = nullptr;
	Vector2 playerHPBARPos_;
	Vector4 playerHPColor_;
	int32_t playerStartHP_;
	int32_t playerHP_ = 0;
	int32_t playerOldHP_;

	std::unique_ptr<Sprite> timeSprite_ = nullptr;
	Vector4 timeColor_;
	int32_t timeGauge_;
	int32_t timeRest_;
	int32_t nowPhase_;

	// 初回イージング用
	float easetime_ = 0.0f;
	int32_t easeTimer_ = 0;
	int32_t easeMaxTime_ = 50;

	bool downPLHp_ ;
	bool isAnimEnd_;
	bool easeHalfOver_;
	bool drawStart_;
};

