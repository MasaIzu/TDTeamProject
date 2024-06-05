#pragma once
#include "Input.h"
#include "Controller.h"
//�@�N���X�̑O���錾
class PlayerAction;

class PlayerActionManager
{
public:

public:
	// �R���X�g���N�^
	PlayerActionManager();
	// �f�X�g���N�^
	~PlayerActionManager();

	//std::list<std::unique_ptr<PlayerBullet>> GetBullets() { return bullets_; }
	//void SetBullets(std::list<std::unique_ptr<PlayerBullet>> bullet) { bullets_ = bullet; }

	void ActionInitialize();

	void ActionUpdate(Input* input, Controller* controller);

	void ActionDraw();

	// �V�[����ύX����
	void ChangeAction(PlayerAction* Action);

public:
	void SetActionNum(int actionNum) { actionNum_ = actionNum; }

	int GetActionNum() { return actionNum_; }

public:
	//WASD�L�[�̂����ꂩ���������Ƃ���true�ɂȂ�֐�
	bool KeyPushWASD(Input* input);

	//L�X�e�B�b�N�������ꂩ�̕����ɓ|�����Ƃ�true�ɂȂ�֐�
	bool isAnyLStick(Controller* controller);

	//bool KeyTriggerArrow(Input* input);

	//bool isAnyLStickOffTrigger(Controller* controller);


	PlayerAction* GetAction() { return action_.get(); }

private:
	std::shared_ptr<PlayerAction> action_;
	int actionNum_;

};

