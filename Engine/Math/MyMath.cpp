#include "MyMath.h"
#include <cmath>
#include"ImGuiManager.h"
#include <random>
#include <stdint.h>

Matrix4 MyMath::Initialize() {
	Matrix4 matInitialize{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return matInitialize;
}


Matrix4 MyMath::Scale(const Vector3& scale) {

	//スケーリング行列を宣言
	Matrix4 matScale = {
		scale.x, 0.0f, 0.0f,   0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f,    0.0f, scale.z, 0.0f,
		0.0f, 0.0f,    0.0f, 1.0f
	};

	Matrix4 scaling = Initialize();
	scaling *= matScale;

	return scaling;
}

Matrix4 MyMath::Rotation(const Vector3& rotation, int X_1_Y_2_Z_3_XYZ_6) {
	int rotation_X = 1;
	int rotation_Y = 2;
	int rotation_Z = 3;

	if (X_1_Y_2_Z_3_XYZ_6 == rotation_X) {
		Matrix4 matRotX = {
		  1.0f,0.0f,0.0f,0.0f,
		  0.0f,cos(rotation.x),sin(rotation.x),0.0f,
		  0.0f,-sin(rotation.x),cos(rotation.x),0.0f,
		  0.0f,0.0f,0.0f,1.0f
		};
		Matrix4 rotationX = Initialize();
		rotationX *= matRotX;

		return rotationX;
	}
	else if (X_1_Y_2_Z_3_XYZ_6 == rotation_Y) {
		Matrix4 matRotY = {
			cos(rotation.y), 0.0f, -sin(rotation.y), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sin(rotation.y), 0.0f, cos(rotation.y),  0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		Matrix4 rotationY = Initialize();
		rotationY *= matRotY;

		return rotationY;
	}
	else if (X_1_Y_2_Z_3_XYZ_6 == rotation_Z) {
		Matrix4 matRotZ = {
		  cos(rotation.z),sin(rotation.z),0.0f,0.0f,
		  -sin(rotation.z),cos(rotation.z),0.0f,0.0f,
		  0.0f,0.0f,1.0f,0.0f,
		  0.0f,0.0f,0.0f,1.0f
		};
		Matrix4 rotationZ = Initialize();
		rotationZ *= matRotZ;

		return rotationZ;
	}
	else {
		Matrix4 matRotXYZ;
		Matrix4 matRot_X, matRot_Y, matRot_Z;
		matRot_X = {
		  1.0f,0.0f,0.0f,0.0f,
		  0.0f,cos(rotation.x),sin(rotation.x),0.0f,
		  0.0f,-sin(rotation.x),cos(rotation.x),0.0f,
		  0.0f,0.0f,0.0f,1.0f
		};
		matRot_Y = {
			cos(rotation.y), 0.0f, -sin(rotation.y),
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			sin(rotation.y), 0.0f, cos(rotation.y),
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};
		matRot_Z = {
		  cos(rotation.z),sin(rotation.z),0.0f,0.0f,
		  -sin(rotation.z),cos(rotation.z),0.0f,0.0f,
		  0.0f,0.0f,1.0f,0.0f,
		  0.0f,0.0f,0.0f,1.0f
		};
		//各軸の回転行列を合成
		matRotXYZ = Initialize();

		matRotXYZ *= matRot_X;
		matRotXYZ *= matRot_Y;
		matRotXYZ *= matRot_Z;

		return matRotXYZ;
	}
}

//Vector3 Vector3::vector3Normalize(Vector3& v) {
//	float x, y, z, w;
//
//	w = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
//	x = v.x / w;
//	y = v.y / w;
//	z = v.z / w;
//
//	return Vector3(x, y, z);
//}

Matrix4 MyMath::Translation(const Vector3& Move) {
	Matrix4 matMove = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f,   1.0f,   0.0f,   0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		Move.x, Move.y, Move.z, 1.0f
	};
	Matrix4 move = Initialize();
	move *= matMove;

	return move;
}

Vector3 MyMath::vector3Normalize(const Vector3& v)
{
	float x, y, z, w;

	w = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	x = v.x / w;
	y = v.y / w;
	z = v.z / w;

	return Vector3(x, y, z);
}

//ベクトルと行列の掛け算(出力Vector3)
Vector3 MyMath::MatVector(const Matrix4& matrix4,const Vector3& vector3) {
	Vector3 matVector = { 0,0,0 };

	matVector.x = vector3.x * matrix4.m[0][0] + vector3.y * matrix4.m[1][0] + vector3.z * matrix4.m[2][0];
	matVector.y = vector3.x * matrix4.m[0][1] + vector3.y * matrix4.m[1][1] + vector3.z * matrix4.m[2][1];
	matVector.z = vector3.x * matrix4.m[0][2] + vector3.y * matrix4.m[1][2] + vector3.z * matrix4.m[2][2];

	return matVector;
}

Vector3 MyMath::GetWorldTransform(const Matrix4& matrix4) {
	Vector3 worldVector = { 0,0,0 };

	worldVector.x = matrix4.m[3][0];
	worldVector.y = matrix4.m[3][1];
	worldVector.z = matrix4.m[3][2];

	return worldVector;
}

const Vector3 MyMath::AddVector3(const Vector3 v1, const Vector3 v2) {
	Vector3 V1(v1);

	V1.x += v2.x;
	V1.y += v2.y;
	V1.z += v2.z;

	return V1;
}

Vector3 MyMath::DivVecMat(const Vector3& vector3, const Matrix4& matrix4)
{
	Vector3 V3 = { 0,0,0 };
	float v1 = 0.0f;

	V3.x = vector3.x * matrix4.m[0][0] + vector3.y * matrix4.m[1][0] + vector3.z * matrix4.m[2][0] + 1 * matrix4.m[3][0];
	V3.y = vector3.x * matrix4.m[0][1] + vector3.y * matrix4.m[1][1] + vector3.z * matrix4.m[2][1] + 1 * matrix4.m[3][1];
	V3.z = vector3.x * matrix4.m[0][2] + vector3.y * matrix4.m[1][2] + vector3.z * matrix4.m[2][2] + 1 * matrix4.m[3][2];
	v1 = vector3.x * matrix4.m[0][3] + vector3.y * matrix4.m[1][3] + vector3.z * matrix4.m[2][3] + 1 * matrix4.m[3][3];

	V3.x /= v1;
	V3.y /= v1;
	V3.z /= v1;


	return { V3.x, V3.y, V3.z };
}

Vector3 MyMath::Vec4ToVec3(const Vector4& vec4)
{
	Vector3 Vec3 = { vec4.x,vec4.y,vec4.z };
	return Vec3;
}

Vector4 MyMath::Vec3ToVec4(const Vector3& vec3)
{
	Vector4 vec3ToVec4 = Vector4(vec3.x, vec3.y, vec3.z, 0);
	return vec3ToVec4;
}

Vector4 MyMath::Vec4Mat4Mul(const Vector4& vec, const Matrix4& mat)
{
	Vector4 retVec = {};

	retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + vec.w * mat.m[3][0];

	retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + vec.w * mat.m[3][1];

	retVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + vec.w * mat.m[3][2];

	return retVec;
}

Matrix4 MyMath::setViewportMat(WinApp* window, const Vector3& v) {
	//単位行列の設定
	Matrix4 matViewport = Initialize();
	matViewport.m[0][0] = static_cast<float>(window->window_width) / 2;
	matViewport.m[1][1] = -static_cast<float>(window->window_height) / 2;
	matViewport.m[3][0] = static_cast<float>((window->window_width) / 2) + v.x;
	matViewport.m[3][1] = static_cast<float>((window->window_height) / 2) + v.y;
	return matViewport;
}

Vector4 MyMath::Vec4MulPs(const Vector4& v4_1,const Vector4& v4_2)
{
	Vector4 result{
		v4_1.x * v4_2.x,
		v4_1.y * v4_2.y,
		v4_1.z * v4_2.z,
		v4_1.w * v4_2.w
	};

	return result;
}

Vector4 MyMath::Vec4AddPs(const Vector4& v4_1,const Vector4& v4_2)
{
	Vector4 result{
		v4_1.x + v4_2.x,
		v4_1.y + v4_2.y,
		v4_1.z + v4_2.z,
		v4_1.w + v4_2.w
	};

	return result;
}

Matrix4 MyMath::MatrixInverse(Matrix4& pOut)
{
	Matrix4 mat;
	int i, j, loop;
	double fDat, fDat2;
	double mat_8x4[4][8];
	int flag;
	float* pF;
	double* pD;

	//8 x 4行列に値を入れる
	for (i = 0; i < 4; i++) {
		pF = pOut.m[i];
		for (j = 0; j < 4; j++, pF++) mat_8x4[i][j] = (double)(*pF);
		pD = mat_8x4[i] + 4;
		for (j = 0; j < 4; j++) {
			if (i == j)   *pD = 1.0;
			else         *pD = 0.0;
			pD++;
		}
	}

	flag = 1;
	for (loop = 0; loop < 4; loop++) {
		fDat = mat_8x4[loop][loop];
		if (fDat != 1.0) {
			if (fDat == 0.0) {
				for (i = loop + 1; i < 4; i++) {
					fDat = mat_8x4[i][loop];
					if (fDat != 0.0) break;
				}
				if (i >= 4) {
					flag = 0;
					break;
				}
				//行を入れ替える
				for (j = 0; j < 8; j++) {
					fDat = mat_8x4[i][j];
					mat_8x4[i][j] = mat_8x4[loop][j];
					mat_8x4[loop][j] = fDat;
				}
				fDat = mat_8x4[loop][loop];
			}

			for (i = 0; i < 8; i++) mat_8x4[loop][i] /= fDat;
		}
		for (i = 0; i < 4; i++) {
			if (i != loop) {
				fDat = mat_8x4[i][loop];
				if (fDat != 0.0f) {
					//mat[i][loop]をmat[loop]の行にかけて
					//(mat[j] - mat[loop] * fDat)を計算
					for (j = 0; j < 8; j++) {
						fDat2 = mat_8x4[loop][j] * fDat;
						mat_8x4[i][j] -= fDat2;
					}
				}
			}
		}
	}

	if (flag) {
		for (i = 0; i < 4; i++) {
			pF = mat.m[i];
			pD = mat_8x4[i] + 4;
			for (j = 0; j < 4; j++) {
				*pF = (float)(*pD);
				pF++;
				pD++;
			}
		}
	}
	else {
		//単位行列を求める
		mat = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
		};
	}

	pOut = mat;

	if (flag) return pOut;
	return pOut;
}

Matrix4 MyMath::MakeInverse(const Matrix4* mat)
{

	//掃き出し法を行う行列
	float sweep[4][8]{};
	//定数倍用
	float constTimes = 0.0f;
	//許容する誤差
	float MAX_ERR = 1e-10f;
	//戻り値用
	Matrix4 retMat;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			//weepの左側に逆行列を求める行列をセット
			sweep[i][j] = mat->m[i][j];

			//sweepの右側に単位行列をセット
			sweep[i][4 + j] = MyMath::MakeIdentity().m[i][j];
		}
	}

	//全ての列の対角成分に対する繰り返し
	for (size_t i = 0; i < 4; i++)
	{
		//最大の絶対値を注目対角成分の絶対値と仮定
		float max = std::fabs(sweep[i][i]);
		size_t maxIndex = i;

		//i列目が最大の絶対値となる行を探す
		for (size_t j = i + 1; j < 4; j++)
		{
			if (std::fabs(sweep[j][i]) > max)
			{
				max = std::fabs(sweep[j][i]);
				maxIndex = j;
			}
		}

		if (fabs(sweep[maxIndex][i]) <= MAX_ERR)
		{
			//逆行列は求められない
			return MyMath::MakeIdentity();
		}

		//操作(1):i行目とmaxIndex行目を入れ替える
		if (i != maxIndex)
		{
			for (size_t j = 0; j < 8; j++)
			{
				float tmp = sweep[maxIndex][j];
				sweep[maxIndex][j] = sweep[i][j];
				sweep[i][j] = tmp;
			}
		}

		//sweep[i][i]に掛けると1になる値を求める
		constTimes = 1 / sweep[i][i];

		//操作(2):p行目をa倍する
		for (size_t j = 0; j < 8; j++)
		{
			//これによりsweep[i][i]が1になる
			sweep[i][j] *= constTimes;
		}

		//操作(3)によりi行目以外の行のi列目を0にする
		for (size_t j = 0; j < 4; j++)
		{
			if (j == i)
			{
				//i行目はそのまま
				continue;
			}

			//i行目に掛ける値を求める
			constTimes = -sweep[j][i];

			for (size_t k = 0; k < 8; k++)
			{
				//j行目にi行目をa倍した行を足す
				//これによりsweep[j][i]が0になる
				sweep[j][k] += sweep[i][k] * constTimes;
			}
		}
	}

	//sweepの右半分がmatの逆行列
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			retMat.m[i][j] = sweep[i][4 + j];
		}
	}

	return retMat;
}

Matrix4 MyMath::ConvertXMMATtoMat4(const DirectX::XMMATRIX& XMMatrix) {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {

		result.m[i][0] = DirectX::XMVectorGetX(XMMatrix.r[i]);
		result.m[i][1] = DirectX::XMVectorGetY(XMMatrix.r[i]);
		result.m[i][2] = DirectX::XMVectorGetZ(XMMatrix.r[i]);
		result.m[i][3] = DirectX::XMVectorGetW(XMMatrix.r[i]);
	}


	return result;
}


Matrix4 MyMath::LookAtLH(const Vector3& eye,const Vector3& target,const Vector3& up) {
	Vector3 tar = target;
	Vector3 ey = eye;
	Vector3 zaxis = tar - ey;
	zaxis.normalize();
	Vector3 xaxis = up.cross(zaxis);
	xaxis.normalize();
	Vector3 yaxis = zaxis.cross(xaxis);
	yaxis.normalize();

	Matrix4 LookAt = {
		xaxis.x,	xaxis.y,	xaxis.z,	0,
		yaxis.x,	yaxis.y,	yaxis.z,	0,
		zaxis.x,	zaxis.y,	zaxis.z,	0,
		eye.x,		eye.y,		eye.z,		1
	};

	return LookAt;
}

Matrix4 MyMath::PerspectiveFovLH(const float& fovAngleY,const float& aspectRatio,const float& nearZ,const float& farZ) {

	double h = 1 / tan(fovAngleY * 0.5);
	double w = h / aspectRatio;
	double a = farZ / (farZ - nearZ);
	double b = (-nearZ * farZ) / (farZ - nearZ);

	Matrix4 perspectiveFovLH = {
		static_cast<float>(w),		 0,		 0,		 0,
		0,		 static_cast<float>(h),		 0,		 0,
		0,		 0,		 static_cast<float>(a),		 1,
		0,		 0,		 static_cast<float>(b),		 0
	};


	return perspectiveFovLH;
}

Matrix4 MyMath::MakeIdentity()
{
	Matrix4 me;
	return me;
}

// 値を範囲内に納める
float MyMath::Clamp(const float& Value, const float& low, const float& high)
{
	float val;
	val = Value;
	if (high < Value)
	{
		val = high;
	}

	if (Value < low)
	{
		val = low;
	}

	return val;
}

Matrix4 MyMath::Matrix4Orthographic(
	const float& viewLeft,const float& viewRight,const float& viewBottom,const float& viewTop,const float& nearZ,const float& farZ) {
	assert(fabsf(viewRight - viewLeft) > 0.00001f);
	assert(fabsf(viewTop - viewBottom) > 0.00001f);
	assert(fabsf(farZ - nearZ) > 0.00001f);

	float width = 1.0f / (viewRight - viewLeft);
	float height = 1.0f / (viewTop - viewBottom);
	float fRange = 1.0f / (farZ - nearZ);
	float sx = width * 2.0f;
	float sy = height * 2.0f;
	float sz = fRange;
	float tx = -(viewLeft + viewRight) * width;
	float ty = -(viewTop + viewBottom) * height;
	float tz = -fRange * nearZ;

	Matrix4 m{ sx, 0.0f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, sz, 0.0f, tx, ty, tz, 1.0f };

	return m;
}

//Matrix4 MyMath::AssimpMatrix(aiMatrix4x4 mat4) {
//
//	Matrix4 assimp;
//
//	assimp.m[0][0] = mat4.a1;
//	assimp.m[0][1] = mat4.a2;
//	assimp.m[0][2] = mat4.a3;
//	assimp.m[0][3] = mat4.a4;
//
//	assimp.m[1][0] = mat4.b1;
//	assimp.m[1][1] = mat4.b2;
//	assimp.m[1][2] = mat4.b3;
//	assimp.m[1][3] = mat4.b4;
//
//	assimp.m[2][0] = mat4.c1;
//	assimp.m[2][1] = mat4.c2;
//	assimp.m[2][2] = mat4.c3;
//	assimp.m[2][3] = mat4.c4;
//
//	assimp.m[3][0] = mat4.d1;
//	assimp.m[3][1] = mat4.d2;
//	assimp.m[3][2] = mat4.d3;
//	assimp.m[3][3] = mat4.d4;
//
//	return assimp;
//}
//
//Vector3 MyMath::AssimpVector3(aiVector3D assimpVec3) {
//
//	Vector3 vec3;
//
//	vec3.x = assimpVec3.x;
//	vec3.y = assimpVec3.y;
//	vec3.z = assimpVec3.z;
//
//	return vec3;
//}
//
//Vector4 MyMath::AssimpQuaternionVec4(aiQuaternion assimpaiQuaternion) {
//
//	Vector4 quaternion;
//
//	quaternion.w = assimpaiQuaternion.w;
//	quaternion.x = assimpaiQuaternion.x;
//	quaternion.y = assimpaiQuaternion.y;
//	quaternion.z = assimpaiQuaternion.z;
//
//	return quaternion;
//}
//
//Quaternion MyMath::AssimpQuaternion(aiQuaternion assimpaiQuaternion) {
//
//	Quaternion quaternion;
//
//	quaternion.w = assimpaiQuaternion.w;
//	quaternion.x = assimpaiQuaternion.x;
//	quaternion.y = assimpaiQuaternion.y;
//	quaternion.z = assimpaiQuaternion.z;
//
//	return quaternion;
//}
//
//Vector4 MyMath::QuaternionSlerp(aiQuaternion& assimpaiQuaternion, aiQuaternion& assimpaiQuaternion2, float t) {
//
//	Quaternion assimQ1 = AssimpQuaternion(assimpaiQuaternion);
//	Quaternion assimQ2 = AssimpQuaternion(assimpaiQuaternion2);
//
//	/*assimQ1.Slerp(assimQ2, t);
//
//	Vector4 slrap = { assimQ1.x,assimQ1.y,assimQ1.z,assimQ1.w };*/
//
//
//
//	DirectX::XMVECTOR start = { assimQ1.x, assimQ1.y ,assimQ1.z ,assimQ1.w };
//
//	DirectX::XMVECTOR end = { assimQ2.x, assimQ2.y ,assimQ2.z ,assimQ2.w };
//
//	DirectX::XMVECTOR vOut = DirectX::XMQuaternionSlerp(start, end, t);
//
//	Vector4 slrapKari = { vOut.m128_f32[0],vOut.m128_f32[1],vOut.m128_f32[2],vOut.m128_f32[3] };
//
//	return slrapKari;
//}

bool MyMath::CircleHit(const Vector3& pos1,const Vector3& pos2,const float& Radius,float& Size)
{
	Vector3 tmp;
	tmp = pos2 - pos1;
	float dist = tmp.dot(tmp);
	float radius2 = Radius;
	radius2 *= radius2;

	if ( dist <= radius2 )
	{
		Size = 1.0f - dist / radius2;
		return true;
	}
	else
	{
		Size = 0.0f;
		return false;
	}
}

float MyMath::GetAngle(const float& angle)
{
	return angle * (PI / 180);
}
Vector3 MyMath::GetAngleVec3(const Vector3& angle)
{
	Vector3 Angle;

	Angle.x = angle.x * ( PI / 180 );
	Angle.y = angle.y * ( PI / 180 );
	Angle.z = angle.z * ( PI / 180 );

	return Angle;
}

float MyMath::JudgeLeftorRight(const Vector3& A,const Vector3& B,const Vector3& C) {
	float crossProductZ = ( B.x - A.x ) * ( C.z - A.z ) - ( B.z - A.z ) * ( C.x - A.x );

	float Left = 1.0f;
	float Right = -1.0f;
	float Center = 0.0f;

	if ( crossProductZ > 0.0f )
	{
		//左
		return Left;
	}
	else if ( crossProductZ < 0.0f )
	{
		//右
		return Right;
	}
	else
	{
		//中央
		return Center;
	}
}

float MyMath::GetRadAngle(const float& angle)
{
	return angle * (180 / PI);
}

void MyMath::MatrixText(const Matrix4& mat)
{

	for (int i = 0; i < 4; i++) {
		float M[4] = {};
		for (int j = 0; j < 4; j++) {
			M[j] = mat.m[i][j];
		}
		ImGui::Text("mat%d:%f,%f,%f,%f", i, M[0], M[1], M[2], M[3]);
	}

}


float MyMath::Get2VecAngle(const Vector3& vec1, const Vector3& vec2)
{

	float angle = atan2(vec2.x - vec1.x, vec2.z - vec1.z);

	if (angle < 0) {
		angle = angle + 2 * PI;
	}

	angle = floor(angle * 360 / (2 * PI));

	return angle;
}

float MyMath::Get2VecAngleY(const Vector3& vec1, const Vector3& vec2)
{

	float angle = atan2(vec2.x - vec1.x, vec2.z - vec1.z);

	angle = floor(angle * 360 / (2 * PI));

	return angle;
}

float MyMath::Get2FloatAngle(const float& x, const float& y)
{

	float angle = atan2(x, y);

	if (angle < 0) {
		angle = angle + 2 * PI;
	}

	angle = floor(angle * 360 / (2 * PI));

	return angle;
}

float MyMath::Distance2Vec2(const Vector2& vec1, const Vector2& vec2)
{
	float x = (vec2.x - vec1.x) * (vec2.x - vec1.x);
	float y = (vec2.y - vec1.y) * (vec2.y - vec1.y);
	return sqrtf(x + y);
}

Vector3 MyMath::HorizontalProjection(const Vector3& startSpeed, float& g, float& flame)
{
	Vector3 speed;

	speed = startSpeed;
	//速度を計算(鉛直投げ上げ)
	speed.y = startSpeed.y - g * (static_cast<float>(flame) / 60.0f);


	//フレームを追加
	flame++;

	return speed;
}

const Vector2 MyMath::lerpVec2(const Vector2& start,const Vector2& end,const float t) {

	return start * ( 1.0f - t ) + end * t;
}

const Vector3 MyMath::lerp(const Vector3& start, const Vector3& end, const float t) {

	return start * (1.0f - t) + end * t;
}

float MyMath::Uint32ToFloat(const uint32_t& convert)
{
	float con = static_cast< float >( convert );
	return con;
}

Vector2 MyMath::GetWindowPos(const Matrix4& matView_, const Matrix4& matProjection_, const Vector2& windowWH, const Vector3& Pos, const float& Inversion, bool& IsInversion)
{
	//ビューポート行列
	Matrix4 Viewport =
	{ windowWH.x / 2,0,0,0,
	0,-windowWH.y / 2,0,0,
	0,0,1,0,
	windowWH.x / 2, windowWH.y / 2,0,1 };

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4 matView = matView_;
	Matrix4 matProjection = matProjection_;

	Matrix4 matViewProjectionViewport = matView * matProjection * Viewport;

	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	Vector3 ScreenPos = DivVecMat(Pos, matViewProjectionViewport);

	ScreenPos.x = ScreenPos.x / windowWH.x;
	ScreenPos.y = ScreenPos.y / windowWH.y;

	if (ScreenPos.x < -35.0f) {
		ScreenPos.x = -35.0f;
	}
	else if (ScreenPos.x > 35.0f) {
		ScreenPos.x = 35.0f;
	}

	if (ScreenPos.y < -1.0f) {
		ScreenPos.y = -1.0f;
	}
	else if (ScreenPos.y > 1.0f) {
		ScreenPos.y = 1.0f;
	}

	ImGui::Begin("ScreenPos");

	ImGui::Text("ScreenPosX = %f",ScreenPos.x);
	ImGui::Text("ScreenPosY = %f",ScreenPos.y);

	ImGui::End();


	if (Inversion > 0) {
		IsInversion = false;
	}
	else {
		IsInversion = true;
	}
	return Vector2(ScreenPos.x, ScreenPos.y);
}

uint32_t MyMath::Random(const uint32_t& low, const uint32_t& high)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}

Vector3 MyMath::RandomVec3(const Uint32Vector2& RLowAndHigh, const Uint32Vector2& GLowAndHigh, const Uint32Vector2& BLowAndHigh)
{
	uint32_t vecX = Random(RLowAndHigh.x, RLowAndHigh.y);
	uint32_t vecY = Random(GLowAndHigh.x, GLowAndHigh.y);
	uint32_t vecZ = Random(BLowAndHigh.x, BLowAndHigh.y);

	Vector3 RandVec = castVec(vecX, vecY, vecZ);

	return RandVec;
}

Vector3 MyMath::RandomCenterVec3(const uint32_t& low, const uint32_t& high)
{
	float half = static_cast<float>(high - low) / 2.0f;
	uint32_t vecX = Random(low, high);
	uint32_t vecY = Random(low, high);
	uint32_t vecZ = Random(low, high);

	Vector3 RandVec = castVec(vecX, vecY, vecZ);
	RandVec = Vector3(RandVec.x - half, RandVec.y - half, RandVec.z - half);

	return RandVec;
}

Vector3 MyMath::RandomCenterVec3(const Uint32Vector2& RLowAndHigh, const Uint32Vector2& GLowAndHigh, const Uint32Vector2& BLowAndHigh)
{
	Vector3 Half;
	Half.x = static_cast<float>(RLowAndHigh.y - RLowAndHigh.x) / 2.0f;
	Half.y = static_cast<float>(GLowAndHigh.y - GLowAndHigh.x) / 2.0f;
	Half.z = static_cast<float>(BLowAndHigh.y - BLowAndHigh.x) / 2.0f;

	uint32_t vecX = Random(RLowAndHigh.x, RLowAndHigh.y);
	uint32_t vecY = Random(GLowAndHigh.x, GLowAndHigh.y);
	uint32_t vecZ = Random(BLowAndHigh.x, BLowAndHigh.y);

	Vector3 RandVec = castVec(vecX, vecY, vecZ);
	RandVec = Vector3(RandVec.x - Half.x, RandVec.y - Half.y, RandVec.z - Half.z);

	return RandVec;
}

Vector3 MyMath::RandomCenterVec3Normalize(const uint32_t& low, const uint32_t& high)
{
	float half = static_cast<float>(high - low) / 2.0f;
	uint32_t vecX = Random(low, high);
	uint32_t vecY = Random(low, high);
	uint32_t vecZ = Random(low, high);

	Vector3 RandVec = castVec(vecX ,vecY ,vecZ);
	RandVec = Vector3(RandVec.x - half, RandVec.y - half, RandVec.z - half);
	RandVec.normalize();

	return RandVec;
}

Vector3 MyMath::castVec(const uint32_t& x, const uint32_t& y, const uint32_t& z)
{
	Vector3 castVec = { static_cast<float>(x) ,static_cast<float>(y) ,static_cast<float>(z) };
	return castVec;
}
