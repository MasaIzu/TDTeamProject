#include "WinApp.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <string>
#include<imgui_impl_win32.h>

MY_SUPPRESS_WARNINGS_END

WinApp* WinApp::WinApp_ = nullptr;
const uint32_t WinApp::window_width = 1280;
const uint32_t WinApp::window_height = 720;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	//Imgui用ウィンドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;


	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		//ウインドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return false;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WinApp* WinApp::GetInstance() {
	if (WinApp_ == nullptr)
	{
		WinApp_ = new WinApp();
	}

	return WinApp_;
}


void WinApp::Destroy()
{
	delete WinApp_;
}

Vector2 WinApp::GetWindowSize() const
{
	Vector2 WinSize = { window_width, window_height };

	return WinSize;
}

void WinApp::MakeWindow(std::wstring title) {

	// ウィンドウクラスの設定
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
	w.lpszClassName = title.c_str(); // ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定
	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);
	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, window_width, window_height };
	// 自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(w.lpszClassName, // クラス名
		title.c_str(), // タイトルバーの文字
		WS_OVERLAPPEDWINDOW, // 標準的なウィンドウスタイル
		100, // 表示X座標(OSに任せる)
		50, // 表示Y座標(OSに任せる)
		wrc.right - wrc.left, // ウィンドウ横幅
		wrc.bottom - wrc.top, // ウィンドウ縦幅
		nullptr, // 親ウィンドウハンドル
		nullptr, // メニューハンドル
		w.hInstance, // 呼び出しアプリケーションハンドル
		nullptr); // オプション
	// ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);

}


bool WinApp::ProcessMessage() {
	MSG msg{}; // メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg);  // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}

void WinApp::DeleteGameWindow() {
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}