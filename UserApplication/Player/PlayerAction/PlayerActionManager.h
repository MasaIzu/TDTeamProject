#pragma once
#include "Input.h"
#include "Controller.h"
//　クラスの前方宣言
class PlayerAction;

class PlayerActionManager
{
public:

public:
	// コンストラクタ
	PlayerActionManager();
	// デストラクタ
	~PlayerActionManager();

	//std::list<std::unique_ptr<PlayerBullet>> GetBullets() { return bullets_; }
	//void SetBullets(std::list<std::unique_ptr<PlayerBullet>> bullet) { bullets_ = bullet; }

	void ActionInitialize();

	void ActionUpdate(Input* input, Controller* controller);

	void ActionDraw();

	// シーンを変更する
	void ChangeAction(PlayerAction* Action);

public:
	void SetActionNum(int actionNum) { actionNum_ = actionNum; }

	int GetActionNum() { return actionNum_; }

public:
	//WASDキーのいずれかを押したときにtrueになる関数
	bool KeyPushWASD(Input* input);

	//Lスティックをいずれかの方向に倒したときtrueになる関数
	bool isAnyLStick(Controller* controller);

	//bool KeyTriggerArrow(Input* input);

	//bool isAnyLStickOffTrigger(Controller* controller);


	PlayerAction* GetAction() { return action_.get(); }

private:
	std::shared_ptr<PlayerAction> action_;
	int actionNum_;

};

