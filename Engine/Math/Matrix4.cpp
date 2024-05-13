#include "Matrix4.h"
#include <cmath>
#include <MyMath.h>

Matrix4::Matrix4()
{
	float result[4][4]
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	// 代入
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result[i][j];
		}
	}
}

Matrix4::Matrix4(const DirectX::XMMATRIX& Matrix4)
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m[i][j] = Matrix4.r[i].m128_f32[j];
		}
	}
}

Matrix4::Matrix4(const float& m00,const float& m01,const float& m02,const float& m03,
	const float& m10,const float& m11,const float& m12,const float& m13,
	const float& m20,const float& m21,const float& m22,const float& m23,
	const float& m30,const float& m31,const float& m32,const float& m33)
{
	m[0][0] = m00, m[0][1] = m01, m[0][2] = m02, m[0][3] = m03,
		m[1][0] = m10, m[1][1] = m11, m[1][2] = m12, m[1][3] = m13,
		m[2][0] = m20, m[2][1] = m21, m[2][2] = m22, m[2][3] = m23,
		m[3][0] = m30, m[3][1] = m31, m[3][2] = m32, m[3][3] = m33;
}

//単位行列を求める
Matrix4 Matrix4::identity()
{
	const Matrix4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};

	return *this;
}

//拡大縮小行列を求める
Matrix4 Matrix4::scale(const Vector3& s)
{
	float result[4][4]
	{
		s.x ,0.0f,0.0f,0.0f,
		0.0f,s.y ,0.0f,0.0f,
		0.0f,0.0f,s.z ,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	// 代入
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result[i][j];
		}
	}

	return *this;
}

// x 軸周りの回転行列を求める
Matrix4 Matrix4::rotateX(const float& angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	float result[4][4]
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f, cos, sin,0.0f,
		0.0f,-sin, cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	// 代入
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result[i][j];
		}
	}

	return *this;
}

// y 軸周りの回転行列を求める
Matrix4 Matrix4::rotateY(const float& angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	float result[4][4]
	{
		cos ,0.0f,-sin,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sin ,0.0f, cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	// 代入
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result[i][j];
		}
	}

	return *this;
}

// z 軸周りの回転行列を求める
Matrix4 Matrix4::rotateZ(const float& angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	float result[4][4]
	{
		cos , sin,0.0f,0.0f,
		-sin, cos,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	// 代入
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result[i][j];
		}
	}

	return *this;
}

// 平行移動行列を求める
Matrix4 Matrix4::translate(const Vector3& t)
{
	float result[4][4]
	{
		1.0f,1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		t.x ,t.y ,t.z ,1.0f
	};

	// 代入
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result[i][j];
		}
	}

	return *this;
}


// 座標変換（ベクトルと行列の掛け算）を行うtransform 関数を作成する　(透視変換にも対応している)
Vector3 Matrix4::transform(const Vector3& v, const Matrix4& mat)
{
	float W = v.x * mat.m[0][3] + v.y * mat.m[1][3] + v.z * mat.m[2][3] + mat.m[3][3];
	Vector3 result
	{
		(v.x * mat.m[0][0] + v.y * mat.m[1][0] + v.z * mat.m[2][0] + mat.m[3][0]) / W,
		(v.x * mat.m[0][1] + v.y * mat.m[1][1] + v.z * mat.m[2][1] + mat.m[3][1]) / W,
		(v.x * mat.m[0][2] + v.y * mat.m[1][2] + v.z * mat.m[2][2] + mat.m[3][2]) / W
	};

	return result;
}

DirectX::XMMATRIX Matrix4::MyMatrixToXMMatrix(const Matrix4& Mat)
{
	DirectX::XMMATRIX m1;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m1.r[i].m128_f32[j] = Mat.m[i][j];
		}
	}

	DirectX::XMMATRIX m2;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m2.r[i].m128_f32[j] = m[i][j];
		}
	}

	m1 *= m2;


	return m1;
}

Matrix4 Matrix4::MatMul(const Matrix4& Mat)
{

	Matrix4 tmp;

	Vector4 vX;
	Vector4 vY;
	Vector4 vZ;
	Vector4 vW;

	Vector4 matR0;
	Vector4 matR1;
	Vector4 matR2;
	Vector4 matR3;

	for (size_t i = 0; i < 4; i++)
	{
		vX = { m[i][0],m[i][0],m[i][0],m[i][0] };
		vY = { m[i][1],m[i][1],m[i][1],m[i][1] };
		vZ = { m[i][2],m[i][2],m[i][2],m[i][2] };
		vW = { m[i][3],m[i][3],m[i][3],m[i][3] };

		matR0 = { Mat.m[0][0],Mat.m[0][1],Mat.m[0][2],Mat.m[0][3] };
		matR1 = { Mat.m[1][0],Mat.m[1][1],Mat.m[1][2],Mat.m[1][3] };
		matR2 = { Mat.m[2][0],Mat.m[2][1],Mat.m[2][2],Mat.m[2][3] };
		matR3 = { Mat.m[3][0],Mat.m[3][1],Mat.m[3][2],Mat.m[3][3] };

		vX = MyMath::Vec4MulPs(vX, matR0);
		vY = MyMath::Vec4MulPs(vY, matR1);
		vZ = MyMath::Vec4MulPs(vZ, matR2);
		vW = MyMath::Vec4MulPs(vW, matR3);

		vX = MyMath::Vec4AddPs(vX, vZ);
		vY = MyMath::Vec4AddPs(vY, vW);
		vX = MyMath::Vec4AddPs(vX, vY);

		tmp.m[i][0] = vX.x;
		tmp.m[i][1] = vX.y;
		tmp.m[i][2] = vX.z;
		tmp.m[i][3] = vX.w;
	}

	return tmp;
}

// 代入演算子　*=　オーバーロード関数（行列と行列の積）
Matrix4& Matrix4::operator*=(const Matrix4& m1)
{
	Matrix4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float Total = 0.0f;
			for (int k = 0; k < 4; k++) {
				Total += m[i][k] * m1.m[k][j];
			}
			result.m[i][j] = Total;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = result.m[i][j];
		}
	}

	return *this;
}


// ２項演算子　*　のオーバーロード関数（行列と行列の積）
Matrix4 Matrix4::operator*(const Matrix4& m1)
{
	return *this *= m1;
}

// ２項演算子　*　のオーバーロード関数（ベクトルと行列の積）
Vector3 operator*(const Vector3& v, const Matrix4& m)
{
	float W = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	Vector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / W,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / W,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / W
	};
	return result;
}