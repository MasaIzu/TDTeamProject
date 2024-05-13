#pragma once

#include "Vector2.h"
#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN

#include<Windows.h>
#include <cstdint>
#include <string>

MY_SUPPRESS_WARNINGS_END

/// <summary>
/// ウィンドウ生成
/// </summary>
class WinApp
{
public:

	// ウィンドウサイズ
	static const uint32_t window_width; // 横幅
	static const uint32_t window_height; // 縦幅

public: // 静的メンバ関数

	// シングルトンインスタンスの取得
	static WinApp* GetInstance();

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public://メンバ関数
	//ウィンドウ作成
	void MakeWindow(std::wstring title);
	//プロセスメッセージ
	bool ProcessMessage();
	//デリートゲームウィンドウ
	void DeleteGameWindow();
	//終了処理
	void Destroy();
	//Hインスタンスゲット
	HINSTANCE GetHInstance() const { return w.hInstance; }
	//ウィンドウサイズ
	Vector2 GetWindowSize() const;

public://GetterSetter
	//hwndゲット
	HWND Gethwnd() { return hwnd; }

private://メンバ変数
	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

	WNDCLASSEX w{};
	HWND hwnd;

private:
	static WinApp* WinApp_;

};

