#pragma once
#include"CollisionPrimitive.h"
#include "RaycastHit.h"

#include "QueryCallback.h"

#include"Matrix4.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <d3d12.h>
#include <forward_list>
MY_SUPPRESS_WARNINGS_END

class BaseCollider;

/// <summary>
/// コリジョンマネージャー
/// </summary>
class CollisionManager {
public:
	static CollisionManager* GetInstance();

public://メンバ関数
	//コライダーの追加
	inline void AddCollider(BaseCollider* collider) {
		colliders.push_front(collider);
	}
	//コライダーを消す
	inline void RemoveCollider(BaseCollider* collide) {
		colliders.remove(collide);
	}
	//コライダークリア
	inline void AllClearCollider() {
		colliders.clear();
	}

	//全てのコリジョンをチェック
	void CheckAllCollisions();

	//レイキャスト
	bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);


	//レイキャストオーバーロード
	bool Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo = nullptr,const float& maxDistance = D3D12_FLOAT32_MAX);

	//QuerySphereの判定
	void QuerySphere(const Sphere& sphere, QueryCallback* callback, unsigned short attribute = static_cast<unsigned short>(0xffffffff));

	bool DetectCollision(const Sphere& sphereA,const Sphere& sphereB,Vector3& out_collision_depth_direction);

	Vector3 ResolveCollision(const Sphere& sphereA,const Sphere& sphereB);

	//敵に当たった
	bool GetIsEnemyHit() { return isEnemyHit; }
	//攻撃が当たった
	bool GetIsAttackHit() { return isAttackHit; }
	//雑魚敵の攻撃が当たった
	bool GetIsWakeEnemyAttackHit() { return isWakeEnemyAttackHit; }
	//当たったナンバーを表示
	int GetHitNumber() { return hitNumber; }
	//敵のポジションをゲット
	Matrix4 GetEnemyWorldPos() { return EnemyWorldPos; }
	//ヒットしたもののポジションをゲット
	Matrix4 GetAttackHitWorldPos() { return HitWorldPos; }

	void ResetMeleeAttack() {
		isMeleeAttackHit = false;
	}

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator = (const CollisionManager&) = delete;

	//コライダーのリスト
	std::forward_list<BaseCollider*> colliders;

	bool isEnemyHit = false;
	bool isAttackHit = false;
	bool isMeleeAttackHit = false;
	int hitNumber = 0;
	bool isWakeEnemyAttackHit = false;
	Matrix4 EnemyWorldPos;
	Matrix4 HitWorldPos;
};