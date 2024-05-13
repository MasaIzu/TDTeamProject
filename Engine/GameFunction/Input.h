#pragma once

#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定

#include "WinApp.h"
#include "Mouse.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <dinput.h>
#include <DirectXMath.h>
#include <Windows.h>
#include <array>
#include <vector>
#include <wrl.h>
MY_SUPPRESS_WARNINGS_END
#include "Controller.h"


// 入力
class Input
{

public: // メンバ関数

	static Input* GetInstance();
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// キーの押したかをチェック(長押し)
	bool PushKey(const BYTE& keyNumber);

	// キーのトリガーをチェック(押した瞬間)
	bool TriggerKey(const BYTE& keyNumber);

	// キーのトリガーをチェック(離した瞬間)
	bool ReleasedKey(const BYTE& keyNumber);

	// マウスボタンのトリガー入力
	bool MouseInputTrigger(const int& button);


	// マウスボタンの入力
	bool MouseInputing(const int& button);


	// マウスボタンの離した瞬間
	bool MouseOffTrigger(const int& button);

	// マウスの位置
	const Vector2 GetMousePos()const;

	// マウスの位置
	const Vector3 GetMouseMove();

	// マウスの位置
	const float GetMouseWheelMove();

	//終了処理
	void Destroy();


	//----- コントローラ- ------//

	//コントローラーの接続があるか
	bool GetIsControllerConnection();

	/// <summary>
	/// コントローラーボタンのトリガー入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool PButtonTrigger(ControllerButton button);

	/// <summary>
	/// コントローラースティックのトリガー入力
	/// </summary>
	/// <param name="stickInput">コントローラースティック方向</param>
	/// <param name="deadRange">デッドゾーンの範囲</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>倒したかどうか</returns>
	bool PStickTrigger(ControllerStick stickInput,const float& deadRange = 0.3f,const Vector2& deadRate = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーボタンの入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool ButtonInput(ControllerButton button);

	/// <summary>
	/// コントローラースティックの入力
	/// </summary>
	/// <param name="stickInput">コントローラースティック方向</param>
	/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>倒したかどうか</returns>
	bool StickInput(ControllerStick stickInput,const float& deadRange = 0.3f,const Vector2& deadRate = { 1.0f,1.0f });


	/// <summary>
	/// コントローラー左スティックの入力
	/// </summary>
	/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>倒したかどうか</returns>
	bool LeftStickInput(const float& deadRange = 0.3f);

	/// <summary>
	/// コントローラーボタンの離した瞬間
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>離したか</returns>
	bool ButtonOffTrigger(ControllerButton button);

	/// <summary>
	/// コントローラースティックの離した瞬間
	/// </summary>
	/// <param name="stickInput">コントローラースティック方向</param>
	/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>離したか</returns>
	bool StickOffTrigger(ControllerStick stickInput,const float& deadRange = 0.3f,const Vector2& deadRate = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーの左スティックのベクトル
	/// </summary>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>ベクトル</returns>
	Vector2 GetLeftStickVec(const Vector2& deadRate = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーの右スティックのベクトル
	/// </summary>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>ベクトル</returns>
	Vector2 GetRightStickVec(const Vector2& deadRate = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーを振動させる
	/// </summary>
	/// <param name="power">振動の強さ0.0f～1.0f</param>
	/// <param name="span">振動の時間フレーム</param>
	void ShakeController(const float& power,const int& span);

private:
	Input() = default;
	~Input();
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;

private:// 静的メンバ変数

	static Input* Input_;

private:
	// DirectInputのインスタンス
	Microsoft::WRL::ComPtr<IDirectInput8> dInput_ = nullptr;
	// キーボードのデバイス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> devKeyboard_ = nullptr;

	//コントローラー
	std::unique_ptr<Controller> controller;

	// 全キーの状態
	BYTE key[256] = {};

	// 前回の全キーの状態
	BYTE keyPre[256] = {};

	// WindowsAPI
	WinApp* winApp_ = nullptr;

	HWND hwnd_;

	//マウス
	std::unique_ptr<Mouse> mouse;
};
