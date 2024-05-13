#include "DebugCamera.h"
#include <windef.h>
#include "WinApp.h"
#include "MyMath.h"
#include"ImGuiManager.h"
#include <SphereCollider.h>
#include <CollisionManager.h>
#include <CollisionAttribute.h>


DebugCamera::DebugCamera()
{
	input_ = Input::GetInstance();
}

DebugCamera::~DebugCamera()
{

}

void DebugCamera::Initialize(ViewProjection* viewProjection_) {

	viewProjection = viewProjection_;
	CameraUpdate();
}


void DebugCamera::Update() {

	if ( input_->MouseInputing(1) )
	{
		CameraUpdate();
	}
	else
	{
		isPushMouse = false;
	}

	if ( input_->GetMouseWheelMove() != 0 )
	{
		cameraDistance_ -= input_->GetMouseWheelMove() / 5.0f;
		cameraDistance_ = max(cameraDistance_,1.0f);
	}

	//ワールド前方ベクトル
	Vector3 forward(0,0,cameraDistance_);
	forward = MyMath::MatVector(CameraRot,forward);//レールカメラの回転を反映
	forward.normalize();

	eye = target + ( forward * cameraDistance_ );

	viewProjection->target = target;
	viewProjection->eye = eye;
	viewProjection->UpdateMatrix();

}

void DebugCamera::CameraUpdate()
{
	POINT mousePosition;
		//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->Gethwnd();
	ScreenToClient(hwnd,&mousePosition);

	if ( isPushMouse == false )
	{
		isPushMouse = true;
		OldPos = Vector2(static_cast< float >( mousePosition.y ),static_cast< float >( mousePosition.x ));
	}

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd,&windowInfo);

	//マウスの移動量を取得
	MouseMove = Vector2(0,0);
	MouseMove = Vector2(static_cast< float >( mousePosition.y ),static_cast< float >( mousePosition.x )) - OldPos;
	mouseMoved += Vector2(MouseMove.x,MouseMove.y) / ( 318.5f + kand );
	HowMachMovePointer += static_cast< uint32_t >( MouseMove.y );

	Vector3 rotation = Vector3(-mouseMoved.x,mouseMoved.y,0);
	Matrix4 cameraRot;
	cameraRot = MyMath::Rotation(rotation,6);

	rot = rotation;
	CameraRot = cameraRot;
	LerpPlayerPosition = MyMath::lerp(LerpPlayerPosition,CameraPos,LerpStrength);
	target = LerpPlayerPosition;

	OldPos = Vector2(static_cast< float >( mousePosition.y ),static_cast< float >( mousePosition.x ));
}
