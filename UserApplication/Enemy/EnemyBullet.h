#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class EnemyBullet
{
public:
	EnemyBullet();
	~EnemyBullet();
	///<summary>
	///初期化
	///</summary>

	void Initialize(const Vector3& position, const Vector3& velocity, Model* bulletModel, ViewProjection* viewProjection_);



	///<summary>
	///更新
	///</summary>

	void Update();

	///<summary>
	///描画
	///</summary>

	void Draw(const ViewProjection& LightViewProjection_);

	///<summary>
	////弾消滅
	///</summary>
	bool IsDead()const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	///<summary>
	////敵の座標
	///</summary>
	Vector3 GetWorldPosition();

	///<summary>
	////弾のサイズを変える
	///</summary>
	void SetSize(Vector3 Size) { worldTransform_.scale_ = Size; }

	///<summary>
	////弾の進行方向に弾に向きを変える
	///</summary>
	void VecRot();

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	std::unique_ptr<Model> model_;// 3Dモデル

	//速度
	Vector3 velocity_;

	//寿命<fim>
	static const int32_t kLifeTime = 60 * 30;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	int32_t deathTimerEnd_ = 0;
	//デスフラグ
	bool isDead_ = false;

private:

};