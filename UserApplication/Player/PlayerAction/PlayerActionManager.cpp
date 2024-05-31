#include "PlayerActionManager.h"

#include "PlayerAction.h"
#include "PlayerMove.h"
#include "PlayerWeekAttack.h"
#include "PlayerHeavyAttack.h"

PlayerActionManager::PlayerActionManager()
{
}

PlayerActionManager::~PlayerActionManager()
{
}

void PlayerActionManager::ActionInitialize()
{
	action_->Initialize();
}

void PlayerActionManager::ActionUpdate(Input* input, Controller* controller)
{

	if (KeyPushWASD(input) == true || isAnyLStick(controller))
	{
		ChangeAction(new PlayerMove(&*this));
	}
	if (input->TriggerKey(DIK_SPACE) || controller->ButtonTrigger(Y))
	{
		ChangeAction(new PlayerHeavyAttack(&*this));
	}
	if (input->TriggerKey(DIK_J) || controller->ButtonTrigger(A))
	{
		ChangeAction(new PlayerWeekAttack(&*this));
	}
	action_->Update(input, controller);
}

void PlayerActionManager::ActionDraw()
{
	action_->Draw();
}

void PlayerActionManager::ChangeAction(PlayerAction* Action)
{
	action_.reset(Action);
}

bool PlayerActionManager::KeyPushWASD(Input* input)
{
	if (input->PushKey(DIK_W) || input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D)) {
		return true;
	}
	else
	{
		return false;
	}
}

bool PlayerActionManager::isAnyLStick(Controller* controller)
{
	if (controller->StickInput(L_UP) || controller->StickInput(L_LEFT) || controller->StickInput(L_DOWN) || controller->StickInput(L_RIGHT)) {
		return true;
	}
	else
	{
		return  false;
	}
}
