#include "PlayerWeekAttack.h"

PlayerWeekAttack::PlayerWeekAttack(PlayerActionManager* ActionManager)
{
}

PlayerWeekAttack::~PlayerWeekAttack()
{
}

void PlayerWeekAttack::Initialize()
{
}

void PlayerWeekAttack::Update(Input* input, Controller* controller)
{
	if (input->TriggerKey(DIK_J) || controller->ButtonTrigger(A))
	{

	}
}

void PlayerWeekAttack::Draw()
{
}
