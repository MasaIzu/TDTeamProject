#pragma once
#include "CollisionPrimitive.h"
#include "Vector4.h"
#include "Vector3.h"

/// <summary>
/// 当たり判定ヘルパークラス
/// </summary>
class Collision
{
public:
	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点（出力用）</param>
	static void ClosestPtPoint2Triangle(const Vector4& point, const Triangle& triangle, Vector4* closest);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphereA">球A</param>
	/// <param name="sphereB">球B</param>
	/// <param name="inter">交点（出力用）</param>
	/// <param name="reject">排斥ベクトル（出力用）</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector4* inter = nullptr, Vector4* reject = nullptr);
	static bool CheckSphere2SphereFastSpeedVer(const Sphere& sphereA, const Sphere& sphereB, const Sphere& checkFastSphere, const uint32_t& whichSpeher);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点（平面上の最近接点）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(const Sphere& sphere,const Plane& plane, Vector4* inter = nullptr);

	/// <summary>
	/// 球と法線付き三角形の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点（三角形上の最近接点）</param>
	/// <param name="reject">排斥ベクトル（出力用）</param>
	/// <returns>交差しているか否か</returns>
	//static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, AliceMathF::Vector4* inter = nullptr);
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector4* inter = nullptr, Vector4* reject = nullptr);

	///// <summary>
	///// 球と法線付き三角形の当たり判定
	///// </summary>
	///// <param name="sphere">球</param>
	///// <param name="triangle">三角形</param>
	///// <param name="inter">交点（三角形上の最近接点）</param>
	///// <param name="reject">排斥ベクトル（出力用）</param>
	///// <returns>交差しているか否か</returns>
	////static bool CheckSphere2Triangle(Sphere& sphere, Triangle& triangle, AliceMathF::Vector4* inter = nullptr, AliceMathF::Vector4* reject = nullptr);


	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector4* inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector4* inter = nullptr);

	///// <summary>
	///// レイと法線付き三角形の当たり判定
	///// </summary>
	///// <param name="lay">レイ</param>
	///// <param name="triangle">三角形</param>
	///// <param name="inter">距離（出力用）</param>
	///// <param name="inter">交点（出力用）</param>
	///// <returns>交差しているか否か</returns>
	//static bool CheckRay2Triangle(Ray& lay, Triangle& triangle, float* distance = nullptr, AliceMathF::Vector4* inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="sphere">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector4* inter = nullptr);

	///// <summary>
	///// レイキャストと球の当たり判定
	///// </summary>
	///// <param name="lay">レイ</param>
	///// <param name="sphere">球</param>
	///// <param name="sphere">距離（出力用）</param>
	///// <param name="inter">交点（出力用）</param>
	///// <returns>交差しているか否か</returns>
	//static bool CheckRayCast2Sphere(RayCast& lay, Sphere& sphere, float* distance = nullptr, AliceMathF::Vector4* inter = nullptr);

	///// <summary>
	///// OBBとOBBの当たり判定
	///// </summary>
	///// <param name="obb1">OBB</param>
	///// <param name="obb2">OBB</param>
	///// <returns>交差しているか否か</returns>
	//static bool CheckOBB2OBB(OBB& obb1, OBB& obb2);

};