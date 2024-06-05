#pragma once
#include "PlayerActionManager.h"
#include "Input.h"
#include "Controller.h"
class PlayerAction
{
public:
	PlayerAction();
	virtual ~PlayerAction();

	virtual void Initialize() = 0;

	virtual void Update(Input* input, Controller* controller) = 0;

	virtual void Draw() = 0;

protected:
	PlayerActionManager* actionManager_;
};

