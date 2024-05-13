#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"

#include "Matrix4.h"

/// <summary>
/// コライダー基底クラス
/// </summary>
class BaseCollider
{
public:
	friend class CollisionManager;
	BaseCollider() = default;
	virtual ~BaseCollider() = default;


	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(const Matrix4& worldPos) = 0;
	virtual void Update(const Matrix4& worldPos, const float& radius) = 0;
	virtual void Update(const Matrix4& worldPos, const float& radius, const float& speed, const Vector3& look) = 0;
	virtual void Update(const Matrix4& worldPos, const uint32_t& Cooltime, const bool& isCoolTime) = 0;
	virtual void Update(const Matrix4& worldPos, const uint32_t& Cooltime, const uint32_t& FirstCoolTime, const bool& isCoolTime) = 0;
	virtual void Update(const Matrix4& worldPos, const float& radius, const uint32_t& Cooltime) = 0;
	virtual void Update(const Matrix4& worldPos, const float& radius, const uint32_t& Cooltime, const bool& isCoolTime) = 0;
	virtual void Update(const Matrix4& worldPos, const float& radius, const uint32_t& Cooltime, const uint32_t& FirstCoolTime, const bool& isCoolTime) = 0;
	//スフィアタイプゲット
	inline CollisionShapeType GetShapeType() { return shapeType; }
	//当たり判定属性ゲット
	inline unsigned short GetAttribute() { return attribute; }
	//プレイヤーの攻撃が当たった
	inline bool GetHit() { return isHitPlayerAttack; }
	//プレイヤーの近接攻撃が当たった
	inline bool GetMeleeHit() { return isHitPlayerMeleeAttack; }
	//スフィア衝突
	inline bool GetHitSphere() { return isHitSphere; }
	//敵同士が接触した
	inline bool GetHitEnemyEachOtherHit() { return isEnemyHittingEachOther; }
	//スフィアに当たった
	inline bool GetSphereMeshHit() { return isSphereMeshHit; }
	//最初のスプラインの場所に当たった
	inline bool GetFirstSplineHit() { return isHitFirstSplineRail; }
	//最後のスプラインの場所に当たった
	inline bool GetFinalSplineHit() { return isHitFinalSplineRail; }
	//Ultにあたった
	inline bool GetIsUltHit() { return isUltHit; }
	//UltSafeZoneにあたった
	inline bool GetIsUltSafeZoneHit() { return isUltSafeZoneHit; }

	//排斥ベクトルの位置ゲット
	inline Vector3 GetRejectVec() { return rejectVec; }
	//inrerベクトルの位置ゲット
	inline Vector4 GetInterVec() { return inter; }
	//位置ゲット
	inline Matrix4 GetWorldPos() { return worldPos_; }


	/// <summary>
	/// 当たり判定属性をセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void SetAttribute(unsigned short Attribute) {
		this->attribute = Attribute;
	}

	/// <summary>
	/// 雑魚敵当たり判定属性をセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void SetAttributeWakeEnemy(unsigned short Attribute) {
		this->attributeWakeEnemy = Attribute;
	}

	/// <summary>
	/// 当たり判定属性を追加
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void AddAttribute(unsigned short Attribute) {
		this->attribute |= Attribute;
	}


	/// <summary>
	/// 当たり判定属性を削除
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void RemoveAttribute(unsigned short Attribute) {
		this->attribute &= !Attribute;
	}

	/// <summary>
	/// ヒットしたのをリセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void Reset() {
		isHitPlayerAttack = false;
	}

	/// <summary>
	/// ヒットしたのをリセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void ResetMeleeHit() {
		isHitPlayerMeleeAttack = false;
	}

	/// <summary>
	/// ヒットしたのをリセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void ResetSphere() {
		isHitSphere = false;
	}

	/// <summary>
	/// ヒットしたのをリセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void ResetUlt() {
		isUltHit = false;
	}

	/// <summary>
	/// ヒットしたのをリセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void ResetUltSafeZone() {
		isUltSafeZoneHit = false;
	}

	/// <summary>
	/// 排斥ベクトルのヒットをリセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void EnemyHittingEachOtherReset() {
		isEnemyHittingEachOther = false;
	}
	/// <summary>
	/// 排斥ベクトルのヒットをリセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void SphereMeshHitReset() {
		isSphereMeshHit = false;
	}
	/// <summary>
	/// 排斥ベクトルのヒットをリセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void FirstSplineHitReset() {
		isHitFirstSplineRail = false;
	}
	/// <summary>
	/// 排斥ベクトルのヒットをリセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void FinalSplineHitReset() {
		isHitFinalSplineRail = false;
	}

protected:
	// 形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	// 当たり判定属性
	unsigned short attribute = 0b1111111111111111;
	// 雑魚的当たり判定属性
	unsigned short attributeWakeEnemy = 0b1111111111111111;

	bool isHitPlayerAttack = false;//プレーヤーの弾が当たっている場合
	bool isHitPlayerMeleeAttack = false;//プレーヤーの弾が当たっている場合
	bool isHitSphere = false;
	bool isEnemyHittingEachOther = false;//敵同士が当たっている場合
	bool isUltHit = false;
	bool isUltSafeZoneHit = false;
	bool isSphereMeshHit = false;
	bool isHitFirstSplineRail = false;
	bool isHitFinalSplineRail = false;

	Vector3 rejectVec;//排斥ベクトル
	Vector4 inter;

	//位置
	Matrix4 worldPos_;
};

