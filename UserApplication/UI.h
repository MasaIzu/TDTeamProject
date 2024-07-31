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
	void Update();
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
	int32_t playerStartHP_;
	int32_t playerHP_ = 0;
	int32_t playerOldHP_;
	Vector4 playerHPColor_;
	bool downPLHp_ ;

	std::unique_ptr<Sprite> timeSprite_ = nullptr;
	Vector4 timeColor_;
	int32_t timeGauge_;
	int32_t nowPhase_;
};

