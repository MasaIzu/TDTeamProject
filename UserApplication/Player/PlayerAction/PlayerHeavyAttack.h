#pragma once
#include "PlayerAction.h"
class PlayerHeavyAttack : public PlayerAction
{
protected:
	PlayerActionManager* ActionManager_;

public:
	PlayerHeavyAttack(PlayerActionManager* ActionManager);
	~PlayerHeavyAttack() override;

	void Initialize() override;
	void Update(Input* input, Controller* controller) override;

	void Draw() override;

private:
};

