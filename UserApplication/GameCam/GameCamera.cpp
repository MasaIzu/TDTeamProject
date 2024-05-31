#include "GameCamera.h"

#include <windef.h>
#include "WinApp.h"
#include "MyMath.h"
#include"ImGuiManager.h"
#include <SphereCollider.h>
#include <CollisionManager.h>
#include <CollisionAttribute.h>

GameCamera::GameCamera()
{
	winWidth = (uint32_t)WinApp::GetInstance()->GetWindowSize().x;
	winHeight = (uint32_t)WinApp::GetInstance()->GetWindowSize().y;

	// 画面サイズに対する相対的なスケールに調整
	scaleX_ = 1.0f / (float)WinApp::GetInstance()->GetWindowSize().x;
	scaleY_ = 1.0f / (float)WinApp::GetInstance()->GetWindowSize().y;

	mousePos = { 0.0f,0.0f };
	oldMousePos = { 0.0f,0.0f };

	MaxCameraTime = 400;
	cameraTime = MaxCameraTime;
	oldMousePos = mousePos;
	mousePos = Input::GetInstance()->GetMousePos();
}

GameCamera::GameCamera(uint32_t window_width, uint32_t window_height)
{
	winWidth = window_width;
	winHeight = window_height;

	// 画面サイズに対する相対的なスケールに調整
	scaleX_ = 1.0f / (float)window_width;
	scaleY_ = 1.0f / (float)window_height;

	mousePos = { 0.0f,0.0f };
	oldMousePos = { 0.0f,0.0f };

	MaxCameraTime = 400;
	cameraTime = MaxCameraTime;
	oldMousePos = mousePos;
	mousePos = Input::GetInstance()->GetMousePos();

}

GameCamera::~GameCamera()
{

}

void GameCamera::Initialize(ViewProjection* viewProjection, const float& cameraAngle, const Vector3& pos) {


	viewProjection_ = viewProjection;
	playerPos_ = pos;
	InitializeCameraPosition(cameraAngle);
	debugCam_ = std::make_unique<DebugCamera>();
	debugCam_->Initialize(viewProjection_);
	FreeCamera = false;

	// コリジョンマネージャに追加
	CameraCollider = new SphereCollider(Vector4(0, CameraCollisionRadius, 0, 0), CameraCollisionRadius);
	CollisionManager::GetInstance()->AddCollider(CameraCollider);
	CameraCollider->SetAttribute(COLLISION_ATTR_CAMERA);
	CameraCollider->Update(MyMath::Translation(viewProjection_->eye));
}

void GameCamera::InitializeCameraPosition(const float& cameraAngle)
{
	windowWH = Uint32Vector2(winWidth / 2, winHeight / 2);

	mouseMoved = Vector2(0, cameraAngle);
	CameraRot = MyMath::MakeIdentity();


	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->Gethwnd();

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	uint32_t xPos_absolute = windowWH.x + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	uint32_t yPos_absolute = windowWH.y + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス
	SetCursorPos(xPos_absolute, yPos_absolute);//移動させる

	target = playerPos_ + cameraHigh;
	//ワールド前方ベクトル
	Vector3 forward(0, 0, cameraDistance_);
	//レールカメラの回転を反映
	forward = MyMath::MatVector(CameraRot, forward);
	forward.normalize();

	//target = pos;
	eye = target + (forward * cameraDistance_);

	if (fovYUpdate == true) {
		fovYUpdate = false;
		viewProjection_->fovAngleY = Fov;
	}
	viewProjection_->target = target;
	viewProjection_->eye = eye;
	viewProjection_->UpdateMatrix();
}

void GameCamera::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_F1)) {
		if (cameraMode == false) {
			cameraMode = true;
		}
		else {
			cameraMode = false;
		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_F2)) {
		if (debugMode == false) {
			debugMode = true;
		}
		else {
			debugMode = false;
		}
	}

	if (FreeCamera == false) {
		{
			//if (cameraMode == false) {
			//	if (Input::GetInstance()->PushKey(DIK_UP)) {
			//		mouseMoved.x += 0.001f;
			//	}
			//	if (Input::GetInstance()->PushKey(DIK_DOWN)) {
			//		mouseMoved.x -= 0.001f;
			//	}
			//	PlaySceneCamera();
			//}
			//else {
			//	SceneCamera();
			//}
			////オブジェクトが挟まったらオブジェクトの前にカメラを持ってくる
			//CheckBetweenToCameraCollider();
			//CameraCollider->Update(MyMath::Translation(eye));
			//if (CameraCollider->GetSphereMeshHit()) {
			//	CameraCollider->SphereMeshHitReset();
			//}
		}

		LookDownCamUpdate();

	}
	else {

		target = SetTargetVec;
		eye = SetEyeVec;

	}

	viewProjection_->target = target;
	viewProjection_->eye = eye;
	viewProjection_->UpdateMatrix();

	if (debugMode == true) {
		debugCam_->Update();
	}
}

void GameCamera::HowMuchMoved()
{
	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->Gethwnd();
	ScreenToClient(hwnd, &mousePosition);

	uint32_t xPos_absolute, yPos_absolute;

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	xPos_absolute = windowWH.x + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	yPos_absolute = windowWH.y + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス
	SetCursorPos(xPos_absolute, yPos_absolute);//移動させる

	//マウスの移動量を取得
	MouseMove = Vector2(0, 0);
	MouseMove = (Vector2(static_cast<float>(mousePosition.y), static_cast<float>(mousePosition.x)) - Uint32Vector2(windowWH.y, windowWH.x));//座標軸で回転している関係でこうなる(XとYが入れ替え)
	Mous_UP_DOWN = Vector2(MouseMove.x, MouseMove.y) / (318.5f + kand);
	mouseMoved += Vector2(MouseMove.x, MouseMove.y) / (318.5f + kand);
	HowMachMovePointer += static_cast<uint32_t>(MouseMove.y);
}

void GameCamera::CheckCameraWhichWayMove()
{
	//どっち向きに移動したのか
	if (Mous_UP_DOWN.x > 0) {
		cameraUp = true;
		cameraDown = false;
	}
	else if (Mous_UP_DOWN.x < 0) {
		cameraUp = false;
		cameraDown = true;
	}
	else {
		cameraUp = false;
		cameraDown = false;
	}
}

void GameCamera::PlaySceneCamera() {

	if (cameraTime < MaxCameraTime) {
		cameraTime++;
	}
	if (shakeTime > 0) {
		shakeTime--;
	}
	else {
		isShake = false;
	}
	//カメラ移動量取得
	HowMuchMoved();
	//どっち向きに移動したのか
	CheckCameraWhichWayMove();

	//カメラ制限
	if (mouseMoved.x < -0.80f) {
		mouseMoved.x = -0.80f;
	}
	else if (mouseMoved.x > 1.30f) {
		mouseMoved.x = 1.30f;
	}


	Vector3 rotation = Vector3(-mouseMoved.x, mouseMoved.y, 0);
	Matrix4 cameraRot;
	cameraRot = MyMath::Rotation(rotation, 6);

	rot = rotation;
	CameraRot = cameraRot;
	LerpPlayerPosition = MyMath::lerp(LerpPlayerPosition, playerPos_, LerpStrength);
	target = LerpPlayerPosition + cameraHigh;

	//ワールド前方ベクトル
	Vector3 forward(0, 0, cameraDistance_);
	forward = MyMath::MatVector(CameraRot, forward);//レールカメラの回転を反映
	forward.normalize();

	//target = pos;
	//eye = target + (forward * cameraDistance_);
	eye = target + (forward * cameraDistance_);
	CameraAngle(eye.z - target.z, eye.x - target.x);

}

void GameCamera::SceneCamera()
{
	target = playerPos_ + cameraHigh;

	//ワールド前方ベクトル
	Vector3 forward(0, 0, cameraDistance_);
	forward = MyMath::MatVector(CameraRot, forward);//レールカメラの回転を反映
	forward.normalize();

	//target = pos;
	//eye = target + (forward * cameraDistance_);

	CameraAngle(eye.z - target.z, eye.x - target.x);
}

void GameCamera::Collision()
{
	isShake = true;
	shakeTime = 10;
}

void GameCamera::MousePositionReset()
{
	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->Gethwnd();
	ScreenToClient(hwnd, &mousePosition);

	uint32_t xPos_absolute, yPos_absolute;

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	xPos_absolute = windowWH.x + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	yPos_absolute = windowWH.y + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス
	SetCursorPos(xPos_absolute, yPos_absolute);//移動させる

}

void GameCamera::MousePositionReset(Vector2& vector2)
{
	//MousePositionReset();

	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->Gethwnd();
	ScreenToClient(hwnd, &mousePosition);

	uint32_t xPos_absolute, yPos_absolute;

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	xPos_absolute = windowWH.x + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	yPos_absolute = windowWH.y + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス
	SetCursorPos(xPos_absolute, yPos_absolute);//移動させる

	//マウスの移動量を取得
	MouseMove = Vector2(0, 0);
	MouseMove = Vector2(vector2.y, vector2.x);
	Mous_UP_DOWN = Vector2(MouseMove.x, MouseMove.y) / (318.5f + kand);
	mouseMoved += Vector2(MouseMove.x, MouseMove.y) / (318.5f + kand);
	HowMachMovePointer += static_cast<uint32_t>(MouseMove.y);

	//カメラ制限
	if (mouseMoved.x < -0.80f) {
		mouseMoved.x = -0.80f;
	}
	else if (mouseMoved.x > 1.30f) {
		mouseMoved.x = 1.30f;
	}


	Vector3 rotation = Vector3(-mouseMoved.x, mouseMoved.y, 0);
	Matrix4 cameraRot;
	cameraRot = MyMath::Rotation(rotation, 6);

	rot = rotation;
	CameraRot = cameraRot;
	target = MyMath::lerp(target, playerPos_ + cameraHigh, 0.005f);

	//ワールド前方ベクトル
	Vector3 forward(0, 0, cameraDistance_);
	forward = MyMath::MatVector(CameraRot, forward);//レールカメラの回転を反映
	forward.normalize();

	//target = pos;
	eye = target + (forward * cameraDistance_);
	CameraAngle(eye.z - target.z, eye.x - target.x);
}

bool GameCamera::CheckBetweenToCameraCollider()
{
	bool isGroundHit = false;

	// 範囲レイキャスト
	Ray GroundRay;
	GroundRay.start = MyMath::Vec3ToVec4(playerPos_);
	//LookRay.start.y += CameraRayCollisionRadius;
	PlayerToCameraVec = (eye - Vector3(0, CameraCollisionRadius, 0)) - playerPos_;
	PlayerToCameraVecDistance = PlayerToCameraVec.length();
	GroundRay.dir = MyMath::Vec3ToVec4(PlayerToCameraVec.norm());
	RaycastHit raycastHit;

	//カメラとの間に地面があれば位置を変える
	if (CollisionManager::GetInstance()->Raycast(GroundRay, COLLISION_ATTR_LANDSHAPE, &raycastHit, PlayerToCameraVecDistance)) {
		Vector3 eyeToInter = eye - MyMath::Vec4ToVec3(raycastHit.inter);
		target = target - eyeToInter;
		eye = MyMath::Vec4ToVec3(raycastHit.inter) + Vector3(0, CameraCollisionRadius, 0);
		PlayerToCameraVec = eye - playerPos_;
		PlayerToCameraVecDistance = PlayerToCameraVec.length();
		isGroundHit = true;
	}

	// 範囲レイキャスト
	Ray LookRay;
	LookRay.start = MyMath::Vec3ToVec4(playerPos_);
	//LookRay.start.y += CameraRayCollisionRadius;
	PlayerToCameraVec = eye - playerPos_;
	PlayerToCameraVecDistance = PlayerToCameraVec.length();
	LookRay.dir = MyMath::Vec3ToVec4(PlayerToCameraVec.norm());
	RaycastHit ObjectRaycastHit;

	//カメラとの間にオブジェクトがあれば位置を変える
	if (CollisionManager::GetInstance()->Raycast(LookRay, COLLISION_ATTR_OBJECT, &ObjectRaycastHit, PlayerToCameraVecDistance)) {
		Vector3 eyeToInter = eye - MyMath::Vec4ToVec3(ObjectRaycastHit.inter);
		target = target - eyeToInter;
		eye = MyMath::Vec4ToVec3(ObjectRaycastHit.inter) + Vector3(0, CameraCollisionRadius, 0);
		PlayerToCameraVec = eye - playerPos_;
		PlayerToCameraVecDistance = PlayerToCameraVec.length();
		isGroundHit = true;
	}

	return isGroundHit;
}

void GameCamera::LookDownCamUpdate()
{
	Vector3 birdCam;
	birdCam = lookDownCamPos_;
	birdCam.y = lookDownCamPos_.y + lookDownCamDistans_;
	birdCam.z -= 1;
	target = lookDownCamPos_;
	eye = birdCam;

}

float GameCamera::GetFovAngle()
{
	return MyMath::GetAngle(Fov);
}

float GameCamera::GetCameraDistanse()
{
	return PlayerToCameraVecDistance;
}

float GameCamera::GetMaxDistance()
{
	return cameraDistance_;
}

Vector2 GameCamera::GetCameraAngle() const
{
	Vector2 CameraAngle;
	CameraAngle.x = mouseMoved.y - MyMath::PI;
	CameraAngle.y = mouseMoved.x;
	return CameraAngle;
}

Vector3 GameCamera::GetEyeToTagetVecDistance(const float& distance) const
{
	Vector3 eyeToTargetVec = target - eye;
	eyeToTargetVec.normalize();

	return eyeToTargetVec * distance + eye;
}

Vector3 GameCamera::GetPlayerDistanceEyePos(const Vector3& playerPos)
{
	Vector3 rotation = Vector3(-mouseMoved.x, mouseMoved.y, 0);
	Matrix4 cameraRot;
	cameraRot = MyMath::Rotation(rotation, 6);

	Vector3 Target = playerPos + cameraHigh;

	//ワールド前方ベクトル
	Vector3 forward(0, 0, cameraDistance_);
	forward = MyMath::MatVector(cameraRot, forward);//レールカメラの回転を反映
	forward.normalize();

	//target = pos;
	Vector3 eye_ = Target + (forward * cameraDistance_);
	return eye_;
}

void GameCamera::SetCameraMode(const bool& mode)
{
	cameraMode = mode;
}

void GameCamera::SetPlayerPosition(const Vector3& pos)
{
	playerPos_ = pos;
}

void GameCamera::SetFreeCamera(const bool& mode)
{
	FreeCamera = mode;
}

void GameCamera::SetCameraTargetAndPos(const Vector3& Target, const Vector3& Eye)
{
	SetTargetVec = Target;
	SetEyeVec = Eye;
}

float GameCamera::FieldOfViewY(const float& focalLengs, const float& sensor)
{
	fovYUpdate = true;
	return float(2 * atan(sensor / (2 * focalLengs)));
}


void GameCamera::CameraAngle(const float& x, const float& z)
{
	angle = atan2(x, z);

	if (angle < 0) {
		angle = angle + 2 * MyMath::PI;
	}

	angle = floor(angle * 360 / (2 * MyMath::PI));

}
