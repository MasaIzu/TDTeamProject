#include "PlayerHeavyAttack.h"

PlayerHeavyAttack::PlayerHeavyAttack(PlayerActionManager* ActionManager)
{
	ActionManager_ = ActionManager;
}

PlayerHeavyAttack::~PlayerHeavyAttack()
{
}

void PlayerHeavyAttack::Initialize()
{
}

void PlayerHeavyAttack::Update(Input* input, Controller* controller)
{
	if (input->TriggerKey(DIK_SPACE)|| controller->ButtonTrigger(Y))
	{

	}
}

void PlayerHeavyAttack::Draw()
{
}
