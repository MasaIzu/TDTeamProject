#pragma once
#include"Vector3.h"
#include"Matrix4.h"
#include "WinApp.h"

#include "Vector4.h"
#include "Quaternion.h"
#include "Uint32Vector2.h"

#include "Defined.h"

MY_SUPPRESS_WARNINGS_BEGIN
#include <DirectXMath.h>
MY_SUPPRESS_WARNINGS_END


//便利系まとめ
namespace MyMath {

	Matrix4 Initialize();
	Matrix4 Scale(const Vector3& scale);
	Matrix4 Rotation(const Vector3& rotation, int X_1_Y_2_Z_3_XYZ_6);
	Matrix4 Translation(const Vector3& move);

	//ベクトルの正規化(いっぺんにやる)
	Vector3 vector3Normalize(const Vector3& v);
	//ベクトルと行列の掛け算(出力Vector3)
	Vector3 MatVector(const Matrix4& matrix4,const Vector3& vector3);

	Vector3 GetWorldTransform(const Matrix4& matrix4);
	const Vector3 AddVector3(const Vector3 v1, const Vector3 v2);
	Vector3 DivVecMat(const Vector3& vector3, const Matrix4& matrix4);

	Vector3 Vec4ToVec3(const Vector4& vec4);

	Vector4 Vec3ToVec4(const Vector3& vec3);
	Vector4 Vec4Mat4Mul(const Vector4& vec, const Matrix4& mat);

	Matrix4 setViewportMat(WinApp* window, const Vector3& v);
	Matrix4 MatrixInverse(Matrix4& pOut);
	Matrix4 MakeInverse(const Matrix4* mat);

	Matrix4 ConvertXMMATtoMat4(const DirectX::XMMATRIX& XMMatrix);

	Matrix4 LookAtLH(const Vector3& eye,const Vector3& target,const Vector3& up);
	Matrix4 PerspectiveFovLH(const float& fovAngleY,const float&  aspectRatio,const float&  nearZ,const float& farZ);

	Matrix4 MakeIdentity();

	float JudgeLeftorRight(const Vector3& A,const Vector3& B,const Vector3& C);

	// 値を範囲内に納める
	float Clamp(const float& Value,const float& low, const float& high);

	Matrix4 Matrix4Orthographic(
		const float& viewLeft,const float& viewRight,const float& viewBottom,const float& viewTop,const float& nearZ,const float& farZ);

	//Matrix4 AssimpMatrix(aiMatrix4x4 mat4);

	//Vector3 AssimpVector3(aiVector3D assimpVec3);

	//Vector4 AssimpQuaternionVec4(aiQuaternion assimpaiQuaternion);
	//Quaternion AssimpQuaternion(aiQuaternion assimpaiQuaternion);

	//Vector4 QuaternionSlerp(aiQuaternion& assimpaiQuaternion, aiQuaternion& assimpaiQuaternion2, float t);

	/// <summary>
	/// サークルの当たり判定をする
	/// </summary>
	/// <param name="pos1"></param>
	/// <param name="pos2"></param>
	/// <param name="Radius"></param>
	/// <param name="Size">どのくらい離れているのか、1が最大0が最小</param>
	/// <returns></returns>
	bool CircleHit(const Vector3& pos1,const Vector3& pos2,const float& Radius,float& Size);

	//度数法からラジアンへ
	float GetAngle(const float& angle);
	Vector3 GetAngleVec3(const Vector3& angle);
	//ラジアンから度数法へ
	float GetRadAngle(const float& angle);

	void MatrixText(const Matrix4& mat);

	Vector4 Vec4MulPs(const Vector4& v4_1,const Vector4& v4_2);
	Vector4 Vec4AddPs(const Vector4& v4_1,const Vector4& v4_2);

	//vec1を中心とした時のvec2の角度を出す
	float Get2VecAngle(const Vector3& vec1, const Vector3& vec2);
	float Get2VecAngleY(const Vector3& vec1, const Vector3& vec2);
	float Get2FloatAngle(const float& x, const float& y);
	float Distance2Vec2(const Vector2& vec1, const Vector2& vec2);

	Vector3 HorizontalProjection(const Vector3& startSpeed, float& g, float& flame);
	const Vector2 lerpVec2(const Vector2& start,const Vector2& end,const float t);
	const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

	float Uint32ToFloat(const uint32_t& convert);

	/// <summary>
	/// 3Dのポジションはウィンドウでどこにあるのが
	/// </summary>
	/// <param name="viewProjection_"></param>
	/// <param name="windowWH">ウィンドウ縦横</param>
	/// <param name="Pos">調べたい3Dのポジション</param>
	/// <param name="Inversion">反転させる値か</param>
	/// <returns></returns>
	Vector2 GetWindowPos(const Matrix4& matView_,const Matrix4& matProjection_,const Vector2& windowWH,const Vector3& Pos,const float& Inversion, bool& IsInversion);

	//ランダム関係
	uint32_t Random(const uint32_t& low, const uint32_t& high);
	Vector3 RandomVec3(const Uint32Vector2& RLowAndHigh, const Uint32Vector2& GLowAndHigh, const Uint32Vector2& BLowAndHigh);
	Vector3 RandomCenterVec3(const uint32_t& low, const uint32_t& high);
	Vector3 RandomCenterVec3(const Uint32Vector2& RLowAndHigh, const Uint32Vector2& GLowAndHigh, const Uint32Vector2& BLowAndHigh);
	Vector3 RandomCenterVec3Normalize(const uint32_t& low, const uint32_t& high);
	Vector3 castVec(const uint32_t& x, const uint32_t& y, const uint32_t& z);

	constexpr float PI = 3.1415926535897931f;

};