#pragma once
#include"ErrorException.h"

#include"Vector2.h"
#include"Vector3.h"
#include"WinApp.h"

#include "MyMath.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <wrl.h>
#include <dinput.h>
MY_SUPPRESS_WARNINGS_END

//入力マウス
class Mouse
{
private:
	//マウス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouseDevice;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 oldMouseState = {};
	//マウスのゲーム空間内でのレイ
	Vector2 mousePos;
	Vector3 mouseMove;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(IDirectInput8* dinput);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// マウスボタンのトリガー入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool MouseInputTrigger(const int& button);

	/// <summary>
	/// マウスボタンの入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool MouseInput(const int& button);

	/// <summary>
	/// マウスボタンの離した瞬間
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>離したか</returns>
	bool MouseOffTrigger(const int& button);

	/// <summary>
	/// マウスの位置
	/// </summary>
	/// <returns>マウスの位置</returns>
	const Vector2 GetMousePos()const;

	/// <summary>
	/// マウスの位置
	/// </summary>
	/// <returns>マウスの位置</returns>
	const Vector3 GetMouseMove();

	/// <summary>
	/// マウスのホイール入力
	/// </summary>
	/// <returns>マウスのホイール入力</returns>
	const float GetMouseWheelMove();

	char PADDING[4];
};