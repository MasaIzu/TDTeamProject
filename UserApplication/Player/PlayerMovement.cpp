#include "PlayerMovement.h"
#include "Numbers.h"
#include <imgui.h>
#include <Easing.h>

PlayerMovement::PlayerMovement()
{
	input_ = Input::GetInstance();
}

PlayerMovement::~PlayerMovement()
{
}

Vector3 PlayerMovement::Move(const PlayerStateNeedMaterial& playerStateNeedMaterial)
{
	playerAllMoveMent = Vec3Number(fNumbers::fZero);

	if (!playerStateNeedMaterial.isBladeAttack && playerStateNeedMaterial.isAlive)
	{
		if (pushKey.isPushMoveKey)
		{
			playerAllMoveMent = playerStateNeedMaterial.worldTransform.LookVelocity.look.norm();

			if (!isRotFinish)
			{
				playerAllMoveMent = playerStateNeedMaterial.worldTransform.LookVelocity.look.norm() * PlayerTrunSpeed;
			}
		}
		else
		{
			if (input_->PushKey(DIK_W) || input_->StickInput(L_UP))
			{
				pushKey.isPushW = true;
				pushKey.isPushMoveKey = true;
				playerAllMoveMent = playerStateNeedMaterial.worldTransform.LookVelocity.look.norm();
			}
			if (input_->PushKey(DIK_S) || input_->StickInput(L_DOWN))
			{
				pushKey.isPushS = true;
				pushKey.isPushMoveKey = true;
				playerAllMoveMent = playerStateNeedMaterial.worldTransform.LookVelocity.lookBack.norm();
			}
			if (input_->PushKey(DIK_A) || input_->StickInput(L_LEFT))
			{
				pushKey.isPushA = true;
				pushKey.isPushMoveKey = true;
				playerAllMoveMent = playerStateNeedMaterial.worldTransform.LookVelocity.lookLeft.norm();
			}
			if (input_->PushKey(DIK_D) || input_->StickInput(L_RIGHT))
			{
				pushKey.isPushD = true;
				pushKey.isPushMoveKey = true;
				playerAllMoveMent = playerStateNeedMaterial.worldTransform.LookVelocity.lookRight.norm();
			}

			if (input_->PushKey(DIK_W) == true && input_->PushKey(DIK_A) == true || input_->StickInput(L_UP) && input_->StickInput(L_LEFT))
			{
				playerAllMoveMent = playerStateNeedMaterial.worldTransform.LookVelocity.look_lookLeft.norm();
			}
			if (input_->PushKey(DIK_W) == true && input_->PushKey(DIK_D) == true || input_->StickInput(L_UP) && input_->StickInput(L_RIGHT))
			{
				playerAllMoveMent = playerStateNeedMaterial.worldTransform.LookVelocity.look_lookRight.norm();
			}
			if (input_->PushKey(DIK_S) == true && input_->PushKey(DIK_A) == true || input_->StickInput(L_DOWN) && input_->StickInput(L_LEFT))
			{
				playerAllMoveMent = playerStateNeedMaterial.worldTransform.LookVelocity.lookBack_lookLeft.norm();
			}
			if (input_->PushKey(DIK_S) == true && input_->PushKey(DIK_D) == true || input_->StickInput(L_DOWN) && input_->StickInput(L_RIGHT))
			{
				playerAllMoveMent = playerStateNeedMaterial.worldTransform.LookVelocity.lookBack_lookRight.norm();
			}
		}
	}

	/*playerAllMoveMent += UpBoost(playerStateNeedMaterial.onGround, playerStateNeedMaterial.isBladeAttack, playerStateNeedMaterial.isAlive);

	BoostFuelUpdate(playerStateNeedMaterial.onGround);*/
	return playerAllMoveMent;
}

Vector3 PlayerMovement::UpBoost(const bool& onGround, const bool& isBladeAttack, const bool& isAlive)
{
	isBoost = false;
	if (!isBladeAttack && isAlive)
	{
		if (input_->PushKey(DIK_SPACE) || input_->ButtonInput(A))
		{
			if (FuelUsedUpBoost <= Fuel)
			{
				isBoost = true;
				FallSpeed = FloatNumber(fNumbers::fZero);
				UpBoostSpeed = max(FallVec.y + UpAcc, UpVYMax);

				isBoostCoolTimeFinish = true;
				BoostCoolTime = MaxBoostCoolTime;
				Fuel -= FuelUsedUpBoost;
			}
		}
	}
	if (!onGround)
	{
		if (!isBoost && !isBladeAttack)
		{
			if (UpBoostSpeed > 0)
			{
				UpBoostSpeed = max(UpBoostSpeed + FallAcc, FallVYMin);
			}
			else
			{
				UpBoostSpeed = FloatNumber(fNumbers::fZero);
				FallSpeed = max(FallSpeed + FallAcc, FallVYMin);
			}
		}
	}
	if (isBladeAttack)
	{
		FallSpeed = FloatNumber(fNumbers::fZero);
		UpBoostSpeed = FloatNumber(fNumbers::fZero);
	}

	FallVec.y = UpBoostSpeed + FallSpeed;

	return FallVec;
}

void PlayerMovement::PlayerAngle()
{
	pushKey.isPushW = false;
	pushKey.isPushA = false;
	pushKey.isPushS = false;
	pushKey.isPushD = false;
	pushKey.isPushMoveKey = false;
	isRotFinish = false;

	if (input_->PushKey(DIK_W) || input_->StickInput(L_UP))
	{
		pushKey.isPushW = true;
		pushKey.isPushMoveKey = true;
		isRotFinish = true;
		PlayerAngleSetter(playerMoveRot.Front);
	}
	if (input_->PushKey(DIK_S) || input_->StickInput(L_DOWN))
	{
		pushKey.isPushS = true;
		pushKey.isPushMoveKey = true;
		isRotFinish = true;
		PlayerAngleSetter(playerMoveRot.Back);
	}
	if (input_->PushKey(DIK_A) || input_->StickInput(L_LEFT))
	{
		pushKey.isPushA = true;
		pushKey.isPushMoveKey = true;
		isRotFinish = true;
		PlayerAngleSetter(playerMoveRot.Left);
	}
	if (input_->PushKey(DIK_D) || input_->StickInput(L_RIGHT))
	{
		pushKey.isPushD = true;
		pushKey.isPushMoveKey = true;
		isRotFinish = true;
		PlayerAngleSetter(playerMoveRot.Right);
	}

	if (input_->PushKey(DIK_W) == true && input_->PushKey(DIK_A) == true || input_->StickInput(L_UP) && input_->StickInput(L_LEFT))
	{
		isRotFinish = true;
		PlayerAngleSetter(playerMoveRot.LeftDiagonal);
	}
	if (input_->PushKey(DIK_W) == true && input_->PushKey(DIK_D) == true || input_->StickInput(L_UP) && input_->StickInput(L_RIGHT))
	{
		isRotFinish = true;
		PlayerAngleSetter(playerMoveRot.FrontDiagonal);
	}
	if (input_->PushKey(DIK_S) == true && input_->PushKey(DIK_A) == true || input_->StickInput(L_DOWN) && input_->StickInput(L_LEFT))
	{
		isRotFinish = true;
		PlayerAngleSetter(playerMoveRot.BackDiagonal);
	}
	if (input_->PushKey(DIK_S) == true && input_->PushKey(DIK_D) == true || input_->StickInput(L_DOWN) && input_->StickInput(L_RIGHT))
	{
		isRotFinish = true;
		PlayerAngleSetter(playerMoveRot.RightDiagonal);
	}
}

void PlayerMovement::CheckSliding()
{
	if (pushKey.isPushW == true && pushKey.isPushA == false && pushKey.isPushD == false)
	{
		SlidingNumber = static_cast<uint32_t>(Numbers::One);
	}
	else if (pushKey.isPushW == false && pushKey.isPushS == false && pushKey.isPushA == true)
	{
		SlidingNumber = static_cast<uint32_t>(Numbers::Two);
	}
	else if (pushKey.isPushS == true && pushKey.isPushA == false && pushKey.isPushD == false)
	{
		SlidingNumber = static_cast<uint32_t>(Numbers::Three);
	}
	else if (pushKey.isPushW == false && pushKey.isPushS == false && pushKey.isPushD == true)
	{
		SlidingNumber = static_cast<uint32_t>(Numbers::Four);
	}
	else if (pushKey.isPushW == true && pushKey.isPushA == true && pushKey.isPushD == false)
	{
		SlidingNumber = static_cast<uint32_t>(Numbers::Five);
	}
	else if (pushKey.isPushW == true && pushKey.isPushA == false && pushKey.isPushD == true)
	{
		SlidingNumber = static_cast<uint32_t>(Numbers::Six);
	}
	else if (pushKey.isPushS == true && pushKey.isPushA == true && pushKey.isPushD == false)
	{
		SlidingNumber = static_cast<uint32_t>(Numbers::Seven);
	}
	else if (pushKey.isPushS == true && pushKey.isPushA == false && pushKey.isPushD == true)
	{
		SlidingNumber = static_cast<uint32_t>(Numbers::Eight);
	}
	else
	{
		SlidingNumber = static_cast<uint32_t>(Numbers::Zero);
	}
}

void PlayerMovement::BoostFuelUpdate(const bool& onGround)
{
	if (isBoostCoolTimeFinish)
	{
		if (BoostCoolTime > 0)
		{
			BoostCoolTime--;
		}
		else
		{
			isBoostCoolTimeFinish = false;
		}
	}
	else
	{
		if (onGround)
		{
			if (Fuel < FuelMax)
			{
				Fuel += FuelRecoveryOnGround;
				if (Fuel > FuelMax)
				{
					Fuel = FuelMax;
				}
			}
		}
		else
		{
			if (Fuel < FuelMax)
			{
				Fuel += FuelRecoveryInTheSky;
				if (Fuel > FuelMax)
				{
					Fuel = FuelMax;
				}
			}
		}
	}
}

bool PlayerMovement::SlidingMaterial(const WorldTransform& worldTransform)
{
	DirectionOfMovement = Vec3Number(fNumbers::fZero);
	isBoostSuccess = false;
	if (FuelUsedBoost <= Fuel)
	{
		isBoostCoolTimeFinish = true;
		isBoostSuccess = true;
		BoostPowerTime = static_cast<uint32_t>(Numbers::Zero);
		BoostPower = BoostStartPower;
		BoostCoolTime = MaxBoostCoolTime;
		Fuel -= FuelUsedBoost;
		if (isPlayerAttack)
		{
			if (SlidingNumber == static_cast<uint32_t>(Numbers::One))
			{
				DirectionOfMovement = worldTransform.LookVelocity.look;
			}
			else if (SlidingNumber == static_cast<uint32_t>(Numbers::Two))
			{
				DirectionOfMovement = worldTransform.LookVelocity.lookLeft;
			}
			else if (SlidingNumber == static_cast<uint32_t>(Numbers::Three))
			{
				DirectionOfMovement = worldTransform.LookVelocity.lookBack;
			}
			else if (SlidingNumber == static_cast<uint32_t>(Numbers::Four))
			{
				DirectionOfMovement = worldTransform.LookVelocity.lookRight;
			}
			else if (SlidingNumber == static_cast<uint32_t>(Numbers::Five))
			{
				DirectionOfMovement = worldTransform.LookVelocity.look_lookLeft;
			}
			else if (SlidingNumber == static_cast<uint32_t>(Numbers::Six))
			{
				DirectionOfMovement = worldTransform.LookVelocity.look_lookRight;
			}
			else if (SlidingNumber == static_cast<uint32_t>(Numbers::Seven))
			{
				DirectionOfMovement = worldTransform.LookVelocity.lookBack_lookLeft;
			}
			else if (SlidingNumber == static_cast<uint32_t>(Numbers::Eight))
			{
				DirectionOfMovement = worldTransform.LookVelocity.lookBack_lookRight;
			}
			else
			{
				DirectionOfMovement = worldTransform.LookVelocity.look;
			}
		}
		else
		{
			DirectionOfMovement = worldTransform.LookVelocity.look;
		}
	}
	return isBoostSuccess;
}

Vector3 PlayerMovement::SlidingUpdate(float& slidingSpeed, const float& maxSlidingSpeed, const uint32_t& Step)
{
	playerSlidingMoveMent = Vec3Number(fNumbers::fZero);

	playerSlidingMoveMent += DirectionOfMovement * slidingSpeed;

	slidingSpeed -= maxSlidingSpeed / static_cast<float>(Step);

	if (BoostPowerTime < BoostPowerMaxTime) {
		BoostPowerTime++;
	}

	BoostPower = Easing::EaseInQuint(BoostStartPower, BoostEndPower, BoostPowerTime, BoostPowerMaxTime);

	return playerSlidingMoveMent;
}

void PlayerMovement::PlayerAngleSetter(const float& angle)
{
	float RotAngle = (PlayerMoveRotation - angle) + playerMoveRot.Back;
	if (RotAngle >= FloatNumber(fNumbers::fZero))
	{
		if (PlayerMoveRotation - angle > playerMoveRot.Back)
		{
			PlayerMoveRotation += playerMoveRot.AddRot;
			if (PlayerMoveRotation >= playerMoveRot.AllRot)
			{
				isRotFinish = false;
				PlayerMoveRotation -= playerMoveRot.AllRot;
			}
		}
		else
		{
			if (PlayerMoveRotation < angle)
			{
				isRotFinish = false;
				PlayerMoveRotation += playerMoveRot.AddRot;
			}
			else if (PlayerMoveRotation > angle)
			{
				isRotFinish = false;
				PlayerMoveRotation -= playerMoveRot.AddRot;
			}
		}
	}
	else
	{
		isRotFinish = false;
		PlayerMoveRotation = playerMoveRot.AllRot - playerMoveRot.AddRot;
	}
}

bool PlayerMovement::GetIsRotFinish()
{
	return isRotFinish;
}

bool PlayerMovement::GetIsPushMoveKey()
{
	return pushKey.isPushMoveKey;
}

bool PlayerMovement::GetIsBoost()
{
	return isBoost;
}

uint32_t PlayerMovement::GetPushBoostKey(const bool& isAttack, const bool& isBladeAttack)
{
	if (isBladeAttack) {
		BoostNumber = static_cast<uint32_t>(Numbers::Nine);
		return BoostNumber;
	}
	if (isAttack)
	{
		if (pushKey.isPushW == true && pushKey.isPushA == false && pushKey.isPushD == false)
		{
			BoostNumber = static_cast<uint32_t>(Numbers::One);
		}
		else if (pushKey.isPushW == false && pushKey.isPushS == false && pushKey.isPushA == true)
		{
			BoostNumber = static_cast<uint32_t>(Numbers::Two);
		}
		else if (pushKey.isPushS == true && pushKey.isPushA == false && pushKey.isPushD == false)
		{
			BoostNumber = static_cast<uint32_t>(Numbers::Three);
		}
		else if (pushKey.isPushW == false && pushKey.isPushS == false && pushKey.isPushD == true)
		{
			BoostNumber = static_cast<uint32_t>(Numbers::Four);
		}
		else if (pushKey.isPushW == true && pushKey.isPushA == true && pushKey.isPushD == false)
		{
			BoostNumber = static_cast<uint32_t>(Numbers::Five);
		}
		else if (pushKey.isPushW == true && pushKey.isPushA == false && pushKey.isPushD == true)
		{
			BoostNumber = static_cast<uint32_t>(Numbers::Six);
		}
		else if (pushKey.isPushS == true && pushKey.isPushA == true && pushKey.isPushD == false)
		{
			BoostNumber = static_cast<uint32_t>(Numbers::Seven);
		}
		else if (pushKey.isPushS == true && pushKey.isPushA == false && pushKey.isPushD == true)
		{
			BoostNumber = static_cast<uint32_t>(Numbers::Eight);
		}
		else
		{
			BoostNumber = static_cast<uint32_t>(Numbers::Zero);
		}
	}
	else
	{
		if (pushKey.isPushW == true || pushKey.isPushA == true || pushKey.isPushD == true || pushKey.isPushS == true)
		{
			BoostNumber = static_cast<uint32_t>(Numbers::One);
		}
		else
		{
			BoostNumber = static_cast<uint32_t>(Numbers::Zero);
		}
	}

	return BoostNumber;
}

float PlayerMovement::GetPlayerAngle()
{
	return PlayerMoveRotation;
}

float PlayerMovement::GetFuel()
{
	return Fuel;
}

float PlayerMovement::GetBoostPower(const bool& isBladeAttack)
{
	if (isBladeAttack) {
		return BoostStartPower;
	}
	return BoostPower;
}

PushKey PlayerMovement::GetPushedKey()
{
	return pushKey;
}
