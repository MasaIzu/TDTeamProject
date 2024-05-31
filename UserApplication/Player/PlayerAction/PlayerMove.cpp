#include "PlayerMove.h"

PlayerMove::PlayerMove(PlayerActionManager* ActionManager)
{
	ActionManager_ = ActionManager;
}

PlayerMove::~PlayerMove()
{
}

void PlayerMove::Initialize()
{
}

void PlayerMove::Update(Input* input, Controller*controller)
{
	if ((input->PushKey(DIK_W) || controller->StickInput(L_UP))||
		(input->PushKey(DIK_A) || controller->StickInput(L_LEFT))||
		(input->PushKey(DIK_S) || controller->StickInput(L_DOWN))||
		(input->PushKey(DIK_D) || controller->StickInput(L_RIGHT)))
	{
		if ((input->PushKey(DIK_W) || controller->StickInput(L_UP)))
		{

		}
		if ((input->PushKey(DIK_A) || controller->StickInput(L_DOWN)))
		{

		}
		if ((input->PushKey(DIK_S) || controller->StickInput(L_LEFT)))
		{

		}
		if ((input->PushKey(DIK_D) || controller->StickInput(L_RIGHT)))
		{

		}
	}
}

void PlayerMove::Draw()
{
}
