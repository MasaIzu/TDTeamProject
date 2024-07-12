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

	void SetPlayerPointer(Player* player) {	player_ = player;}

private:


public:


private:
	std::unique_ptr<Sprite> SpriteTest_ = nullptr;
	Player* player_ = nullptr;

};

