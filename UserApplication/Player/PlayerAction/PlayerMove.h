#pragma once
#include "PlayerAction.h"
class PlayerMove :
    public PlayerAction
{
protected:
	PlayerActionManager* ActionManager_;

public:
	PlayerMove(PlayerActionManager* ActionManager);
	~PlayerMove() override;

	void Initialize() override;
	void Update(Input* input, Controller* controller) override;

	void Draw() override;

private:


};

