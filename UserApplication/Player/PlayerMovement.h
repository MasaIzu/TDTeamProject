#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include <Input.h>
#include "PlayerStruct.h"

class PlayerMovement
{

public://基本関数
	PlayerMovement();
	~PlayerMovement();

	//Move
	Vector3 Move(const PlayerStateNeedMaterial& playerStateNeedMaterial);
	//ブースト
	Vector3 UpBoost(const bool& onGround,const bool& isBladeAttack,const bool& isAlive);
	//Angle
	void PlayerAngle();
	//スライディングマテリアル
	bool SlidingMaterial(const WorldTransform& worldTransform);
	//スライディングアップデート
	Vector3 SlidingUpdate(float& slidingSpeed,const float& maxSlidingSpeed,const uint32_t& Step);

private://メンバ関数

	//アングル
	void PlayerAngleSetter(const float& angle);

	//どっちにスライディングしたのか
	void CheckSliding();

	//ブースト後の燃料アップデート
	void BoostFuelUpdate(const bool& onGround);

public://Setter

public://Getter
	bool GetIsRotFinish();
	bool GetIsPushMoveKey();
	bool GetIsBoost();
	uint32_t GetPushBoostKey(const bool& isAttack,const bool& isBladeAttack);
	float GetPlayerAngle();
	float GetFuel();
	float GetBoostPower(const bool& isBladeAttack);
	PushKey GetPushedKey();
private://クラス関連
	Input* input_ = nullptr;

	PlayerMoveRot playerMoveRot;
	PushKey pushKey;
private://コンスト

	const float FallAcc = -0.035f;
	const float FallVYMin = -1.0f;

	const float UpAcc = 0.0003f;
	const float UpVYMax = 0.7f;
	const float FuelMax = 1000.0f;
private://別クラスから値をもらう

private://クラス変数
	bool isRotFinish = false;
	bool isPlayerAttack = false;
	bool isBoost = false;
	bool isBoostCoolTimeFinish = false;
	bool isBoostSuccess = false;

	uint32_t SlidingNumber = 0;
	uint32_t BoostCoolTime = 0;
	uint32_t MaxBoostCoolTime = 50;
	uint32_t BoostNumber = 0;
	uint32_t BoostPowerMaxTime = 25;
	uint32_t BoostPowerTime = BoostPowerMaxTime;

	float PlayerMoveRotation = 0.0f;
	float PlayerTrunSpeed = 0.4f;
	float UpBoostSpeed = 0.0f;
	float FallSpeed = 0.0f;
	float Fuel = FuelMax;
	float FuelUsedBoost = 150.0f;
	float FuelUsedUpBoost = 5.0f;
	float FuelRecoveryOnGround = 15.0f;
	float FuelRecoveryInTheSky = 1.0f;
	float BoostPower = 0.1f;
	float BoostStartPower = 0.35f;
	float BoostEndPower = 0.15f;

	Vector3 playerAllMoveMent;//移動量
	Vector3 playerSlidingMoveMent;//スライディング移動量
	Vector3 DirectionOfMovement;
	Vector3 FallVec;

};