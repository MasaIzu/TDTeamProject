#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "Input.h"
#include "WinApp.h"
#include <cassert>

#include <basetsd.h>
#include <commctrl.h>
#include <dbt.h>
#include <oleauto.h>
#include <shellapi.h>
#include <wbemidl.h>
#include <wrl/client.h>

#include <dinputd.h>
#include <memory>


Input* Input::Input_ = nullptr;

Input* Input::GetInstance() {
	if (Input_ == nullptr)
	{
		Input_ = new Input();
	}

	return Input_;
}

Input::~Input() {
	if (devKeyboard_) {
		devKeyboard_->Unacquire();
	}

	if (dInput_) {
		dInput_.Reset();
	}

}

void Input::Destroy()
{
	delete Input_;
}

bool Input::GetIsControllerConnection()
{
	return controller->GetIsControllerConnection();
}

void Input::Initialize()
{
	WinApp* winApp = WinApp::GetInstance();

	hwnd_ = winApp->Gethwnd();
	HRESULT result = S_FALSE;

	// DirectInputオブジェクトの生成
	result = DirectInput8Create(
		winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput_, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	result = dInput_->CreateDevice(GUID_SysKeyboard, &devKeyboard_, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = devKeyboard_->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = devKeyboard_->SetCooperativeLevel(
		hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	//マウスデバイスの初期化
	mouse = std::make_unique<Mouse>();
	mouse->Initialize(dInput_.Get());

	controller = std::make_unique<Controller>();
}

void Input::Update()
{
	// 前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	// キーボード情報の取得開始
	devKeyboard_->Acquire();

	// 全キーの入力状態を取得する
	devKeyboard_->GetDeviceState(sizeof(key), key);

	//マウス更新
	mouse->Update();

	//コントローラーデバイスの更新
	controller->Update();
}

bool Input::PushKey(const BYTE& keyNumber)
{
	// 指定したキーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}

	// そうでなければfalseを返す
	return false;
}

bool Input::TriggerKey(const BYTE& keyNumber)
{
	// 指定キーを前フレームで押していなく、今のフレームで押していればtrueを返す
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}

	// そうでなければfalseを返す
	return false;
}

bool Input::ReleasedKey(const BYTE& keyNumber)
{
	// 指定キーを前フレームで押していて、今のフレームで押していなければtrueを返す
	if (keyPre[keyNumber] && !key[keyNumber]) {
		return true;
	}

	// そうでなければfalseを返す
	return false;
}

bool Input::MouseInputTrigger(const int& button)
{
	return mouse->MouseInputTrigger(button);
}

bool Input::MouseInputing(const int& button)
{
	return mouse->MouseInput(button);
}

bool Input::MouseOffTrigger(const int& button)
{
	return mouse->MouseOffTrigger(button);
}

const Vector2 Input::GetMousePos()const
{
	return mouse->GetMousePos();
}

const Vector3 Input::GetMouseMove()
{
	return mouse->GetMouseMove();
}

const float Input::GetMouseWheelMove()
{
	return mouse->GetMouseWheelMove();
}

bool Input::PButtonTrigger(ControllerButton button)
{
	return controller->ButtonTrigger(button);
}

bool Input::PStickTrigger(ControllerStick stickInput,const float& deadRange,const Vector2& deadRate)
{
	return controller->StickTrigger(stickInput,deadRange,deadRate);
}

bool Input::ButtonInput(ControllerButton button)
{
	return controller->ButtonInput(button);
}

bool Input::StickInput(ControllerStick stickInput,const float& deadRange,const Vector2& deadRate)
{
	return controller->StickInput(stickInput,deadRange,deadRate);
}

bool Input::LeftStickInput(const float& deadRange) {
	return controller->LeftStickInput(deadRange);
}

bool Input::ButtonOffTrigger(ControllerButton button)
{
	return controller->ButtonOffTrigger(button);
}

bool Input::StickOffTrigger(ControllerStick stickInput,const float& deadRange,const Vector2& deadRate)
{
	return controller->StickOffTrigger(stickInput,deadRange,deadRate);
}

Vector2 Input::GetLeftStickVec(const Vector2& deadRate)
{
	return  controller->GetLeftStickVec(deadRate);
}

Vector2 Input::GetRightStickVec(const Vector2& deadRate)
{
	return controller->GetRightStickVec(deadRate);
}

void Input::ShakeController(const float& power,const int& span)
{
	controller->ShakeController(power,span);
}
