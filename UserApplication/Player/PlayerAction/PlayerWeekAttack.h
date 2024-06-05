#pragma once
#include "PlayerAction.h"
class PlayerWeekAttack : public PlayerAction
{
protected:
	PlayerActionManager* ActionManager_;

public:
	PlayerWeekAttack(PlayerActionManager* ActionManager);
	~PlayerWeekAttack() override;

	void Initialize() override;
	void Update(Input* input, Controller* controller) override;

	void Draw() override;

private:

};

