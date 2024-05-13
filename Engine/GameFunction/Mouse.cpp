#include "Mouse.h"

void Mouse::Initialize(IDirectInput8* dinput)
{
	HRESULT result;

	//マウスデバイスの生成
	result = dinput->CreateDevice(GUID_SysMouse, &mouseDevice, NULL);
	assert(SUCCEEDED(result));

	result = mouseDevice->SetDataFormat(&c_dfDIMouse2);
	assert(SUCCEEDED(result));

	result = mouseDevice->SetCooperativeLevel(WinApp::GetInstance()->Gethwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}


void Mouse::Update()
{
	HRESULT result;

	//マウス
	result = mouseDevice->Acquire();
	oldMouseState = mouseState;
	result = mouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);

	POINT p;
	GetCursorPos(&p);
	ScreenToClient(FindWindowA("DirectX", nullptr), &p);
	mousePos.x = static_cast<float>(p.x);
	mousePos.x = MyMath::Clamp(mousePos.x, 0.0f, static_cast<float>(WinApp::GetInstance()->window_width));
	mousePos.y = static_cast<float>(p.y);
	mousePos.y = MyMath::Clamp(mousePos.y, 0.0f, static_cast<float>(WinApp::GetInstance()->window_height));
}

bool Mouse::MouseInputTrigger(const int& button)
{
	return (!oldMouseState.rgbButtons[button] && mouseState.rgbButtons[button]);
}

bool Mouse::MouseInput(const int& button)
{
	return static_cast<bool>(mouseState.rgbButtons[button]);
}

bool Mouse::MouseOffTrigger(const int& button)
{
	return (oldMouseState.rgbButtons[button] && !mouseState.rgbButtons[button]);
}

const Vector2 Mouse::GetMousePos() const
{
	return mousePos;
}

const Vector3 Mouse::GetMouseMove()
{
	Vector3 result = { static_cast<float>(mouseState.lX), static_cast<float>(mouseState.lY), static_cast<float>(mouseState.lZ) };
	return result;
}

const float Mouse::GetMouseWheelMove()
{
	return static_cast< float >( mouseState.lZ );
}
