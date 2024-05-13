#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"
#include "MeshCollider.h"
#include <CollisionAttribute.h>

using namespace DirectX;

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::CheckAllCollisions()
{
	isEnemyHit = false;
	isAttackHit = false;
	hitNumber = 0;
	isWakeEnemyAttackHit = false;

	

	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	// 全ての組み合わせについて総当りチェック
	itA = colliders.begin();
	for (; itA != colliders.end(); ++itA) {
		itB = itA;
		++itB;
		for (; itB != colliders.end(); ++itB) {
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			if ( colA->attribute != COLLISION_ATTR_NOTATTACK && colB->attribute != COLLISION_ATTR_NOTATTACK )
			{
				// ともに球
				if ( colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
					colB->GetShapeType() == COLLISIONSHAPE_SPHERE )
				{
					Sphere* SphereA = dynamic_cast< Sphere* >( colA );
					Sphere* SphereB = dynamic_cast< Sphere* >( colB );
					Vector4 inter;
					Vector4 reject;

					if ( colA->attribute == COLLISION_ATTR_ENEMYS && colB->attribute == COLLISION_ATTR_ALLIES ||
						colA->attribute == COLLISION_ATTR_ALLIES && colB->attribute == COLLISION_ATTR_ENEMYS )
					{

						if ( Collision::CheckSphere2Sphere(*SphereA,*SphereB,&inter ,&reject) )
						{
							EnemyWorldPos = colA->GetWorldPos();
							if ( colB->attribute == COLLISION_ATTR_ALLIES )
							{
								colB->rejectVec = ResolveCollision(*SphereA,*SphereB);
								colB->isHitSphere = true;
							}
							else if ( colA->attribute == COLLISION_ATTR_ALLIES )
							{
								colA->rejectVec = ResolveCollision(*SphereB,*SphereA);
								colA->isHitSphere = true;
							}
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_ENEMYS && colB->attribute == COLLISION_ATTR_ENEMYS )
					{
						if ( Collision::CheckSphere2Sphere(*SphereA,*SphereB,&inter,&reject) )
						{
							colA->isEnemyHittingEachOther = true;
							colA->rejectVec = Vector3(reject.x,reject.y,reject.z);
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_ATTACK && colB->attribute == COLLISION_ATTR_ENEMYS )
					{
						if ( Collision::CheckSphere2Sphere(*SphereA,*SphereB,&inter,&reject) )
						{
							HitWorldPos = colA->GetWorldPos();
							colA->isHitPlayerAttack = true;
							colB->isHitPlayerAttack = true;
							//isAttackHit = true;
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_ENEMYS && colB->attribute == COLLISION_ATTR_ATTACK )
					{
						if ( Collision::CheckSphere2Sphere(*SphereA,*SphereB,&inter,&reject) )
						{
							HitWorldPos = colB->GetWorldPos();
							colA->isHitPlayerAttack = true;
							colB->isHitPlayerAttack = true;
							//isAttackHit = true;
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_ENEMY_BULLET_ATTACK && colB->attribute == COLLISION_ATTR_ALLIES ||
						colA->attribute == COLLISION_ATTR_ALLIES && colB->attribute == COLLISION_ATTR_ENEMY_BULLET_ATTACK )
					{
						if ( Collision::CheckSphere2Sphere(*SphereA,*SphereB,&inter) )
						{
							HitWorldPos = colB->GetWorldPos();
							colA->isHitPlayerAttack = true;
							colB->isHitPlayerAttack = true;
							//isAttackHit = true;
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_ALLIES && colB->attribute == COLLISION_ATTR_ENEMY_ULT ||
						colA->attribute == COLLISION_ATTR_ENEMY_ULT && colB->attribute == COLLISION_ATTR_ALLIES )
					{
						if ( Collision::CheckSphere2Sphere(*SphereA,*SphereB,&inter) )
						{
							HitWorldPos = colB->GetWorldPos();
							colA->isUltHit = true;
							colB->isUltHit = true;
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_ALLIES && colB->attribute == COLLISION_ATTR_ENEMY_ULTSAFEZONE ||
						colA->attribute == COLLISION_ATTR_ENEMY_ULTSAFEZONE && colB->attribute == COLLISION_ATTR_ALLIES )
					{
						if ( Collision::CheckSphere2Sphere(*SphereA,*SphereB,&inter) )
						{
							HitWorldPos = colB->GetWorldPos();
							colA->isUltSafeZoneHit = true;
							colB->isUltSafeZoneHit = true;
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_MELEEATTACK && colB->attribute == COLLISION_ATTR_ENEMYS ||
						colA->attribute == COLLISION_ATTR_ENEMYS && colB->attribute == COLLISION_ATTR_MELEEATTACK )
					{
						if ( isMeleeAttackHit == false )
						{
							if ( Collision::CheckSphere2Sphere(*SphereA,*SphereB,&inter) )
							{
								isMeleeAttackHit = true;
								HitWorldPos = colB->GetWorldPos();
								if ( colB->attribute == COLLISION_ATTR_MELEEATTACK )
								{
									colA->isHitPlayerMeleeAttack = true;
									colB->attribute = COLLISION_ATTR_NOTATTACK;
								}
								else if ( colA->attribute == COLLISION_ATTR_MELEEATTACK )
								{
									colA->attribute = COLLISION_ATTR_NOTATTACK;
									colB->isHitPlayerMeleeAttack = true;
								}
							}
						}
						else
						{
							if ( colB->attribute == COLLISION_ATTR_MELEEATTACK )
							{
								colB->attribute = COLLISION_ATTR_NOTATTACK;
							}
							else if ( colA->attribute == COLLISION_ATTR_MELEEATTACK )
							{
								colA->attribute = COLLISION_ATTR_NOTATTACK;
							}
						}
					}
					if ( Collision::CheckSphere2Sphere(*SphereA,*SphereB,&inter) )
					{
						//isEnemyHit = true;
					}
				}
				else if ( colA->GetShapeType() == COLLISIONSHAPE_MESH && colB->GetShapeType() == COLLISIONSHAPE_SPHERE )
				{
					MeshCollider* meshCollider = dynamic_cast< MeshCollider* >( colA );
					Sphere* sphere = dynamic_cast< Sphere* >( colB );
					Vector4 inter;
					if ( colA->attribute == COLLISION_ATTR_LANDSHAPE && colB->attribute == COLLISION_ATTR_ATTACK )
					{
						if ( meshCollider->CheckCollisionSphere(*sphere,&inter,nullptr) )
						{
							colB->isSphereMeshHit = true;
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_LANDSHAPE && colB->attribute == COLLISION_ATTR_ENEMY_BULLET_ATTACK )
					{
						if ( meshCollider->CheckCollisionSphere(*sphere,&inter,nullptr) )
						{
							colB->isSphereMeshHit = true;
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_OBJECT && colB->attribute == COLLISION_ATTR_ATTACK )
					{
						if ( meshCollider->CheckCollisionSphere(*sphere,&inter,nullptr) )
						{
							colB->isSphereMeshHit = true;
						}
					}
					/*else if ( colA->attribute == COLLISION_ATTR_RAIL && colB->attribute == COLLISION_ATTR_ALLIES )
					{
						if ( meshCollider->CheckCollisionSphere(*sphere,&inter,nullptr) )
						{
							colB->isSphereMeshHit = true;
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_FIRSTRAIL && colB->attribute == COLLISION_ATTR_ALLIES )
					{
						if ( meshCollider->CheckCollisionSphere(*sphere,&inter,nullptr) )
						{
							colB->isHitFirstSplineRail = true;
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_FINALRAIL && colB->attribute == COLLISION_ATTR_ALLIES )
					{
						if ( meshCollider->CheckCollisionSphere(*sphere,&inter,nullptr) )
						{
							colB->isHitFinalSplineRail = true;
						}
					}*/
				}
				else if ( colA->GetShapeType() == COLLISIONSHAPE_SPHERE && colB->GetShapeType() == COLLISIONSHAPE_MESH )
				{
					MeshCollider* meshCollider = dynamic_cast< MeshCollider* >( colB );
					Sphere* sphere = dynamic_cast< Sphere* >( colA );
					Vector4 inter;
					if ( colA->attribute == COLLISION_ATTR_ATTACK && colB->attribute == COLLISION_ATTR_LANDSHAPE )
					{
						if ( meshCollider->CheckCollisionSphere(*sphere,&inter,nullptr) )
						{
							colA->isSphereMeshHit = true;
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_ENEMY_BULLET_ATTACK && colB->attribute == COLLISION_ATTR_LANDSHAPE )
					{
						if ( meshCollider->CheckCollisionSphere(*sphere,&inter,nullptr) )
						{
							colA->isSphereMeshHit = true;
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_ATTACK && colB->attribute == COLLISION_ATTR_OBJECT )
					{
						if ( meshCollider->CheckCollisionSphere(*sphere,&inter,nullptr) )
						{
							colA->isSphereMeshHit = true;
						}
					}
					/*else if ( colA->attribute == COLLISION_ATTR_ALLIES && colB->attribute == COLLISION_ATTR_RAIL )
					{
						if ( meshCollider->CheckCollisionSphere(*sphere,&inter,nullptr) )
						{
							colA->isSphereMeshHit = true;
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_ALLIES && colB->attribute == COLLISION_ATTR_FIRSTRAIL )
					{
						if ( meshCollider->CheckCollisionSphere(*sphere,&inter,nullptr) )
						{
							colA->isHitFirstSplineRail = true;
						}
					}
					else if ( colA->attribute == COLLISION_ATTR_ALLIES && colB->attribute == COLLISION_ATTR_FINALRAIL )
					{
						if ( meshCollider->CheckCollisionSphere(*sphere,&inter,nullptr) )
						{
							colA->isHitFinalSplineRail = true;
						}
					}*/
				}
			}
		}
	}
}

bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
{
	return Raycast(ray, 0xffff, hitInfo, maxDistance);
}

bool CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo,const float& maxDistance)
{
	bool result = false;
	std::forward_list<BaseCollider*>::iterator it;
	std::forward_list<BaseCollider*>::iterator it_hit;
	float distance = maxDistance;
	Vector4 inter;

	// 全てのコライダーと総当りチェック
	it = colliders.begin();
	for (; it != colliders.end(); ++it) {
		BaseCollider* colA = *it;

		// 属性が合わなければスキップ
		if (colA->attribute != attribute) {
			continue;
		}

		if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE) {
			Sphere* sphere = dynamic_cast<Sphere*>(colA);

			float tempDistance;
			Vector4 tempInter;

			if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
		else if (colA->GetShapeType() == COLLISIONSHAPE_MESH) {
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);

			float tempDistance;
			Vector4 tempInter;
			if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
	}

	if (result && hitInfo) {
		hitInfo->distance = distance;
		hitInfo->inter = inter;
		hitInfo->collider = *it_hit;
	}

	return result;
}

void CollisionManager::QuerySphere(const Sphere& sphere, QueryCallback* callback, unsigned short attribute)
{
	assert(callback);

	std::forward_list<BaseCollider*>::iterator it;

	// 全てのコライダーと総当りチェック
	it = colliders.begin();
	for (; it != colliders.end(); ++it) {
		BaseCollider* col = *it;

		// 属性が合わなければスキップ
		if (!(col->attribute & attribute)) {
			continue;
		}

		// 球
		if (col->GetShapeType() == COLLISIONSHAPE_SPHERE) {
			Sphere* sphereB = dynamic_cast<Sphere*>(col);

			Vector4 tempInter;
			Vector4 tempReject;
			if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

			// 交差情報をセット
			QueryHit info;
			info.collider = col;
			info.inter = tempInter;
			info.reject = tempReject;

			// クエリーコールバック呼び出し
			if (!callback->OnQueryHit(info)) {
				// 戻り値がfalseの場合、継続せず終了
				return;
			}
		}
		// メッシュ
		else if (col->GetShapeType() == COLLISIONSHAPE_MESH) {
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

			Vector4 tempInter;
			Vector4 tempReject;
			if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

			// 交差情報をセット
			QueryHit info;
			info.collider = col;
			info.inter = tempInter;
			info.reject = tempReject;

			// クエリーコールバック呼び出し
			if (!callback->OnQueryHit(info)) {
				// 戻り値がfalseの場合、継続せず終了
				return;
			}
		}
	}
}

bool CollisionManager::DetectCollision(const Sphere& sphereA,const Sphere& sphereB,Vector3& out_collision_depth_direction)
{
	Vector3 a_center = { sphereA.center.x,sphereA.center.y,sphereA.center.z };
	Vector3 b_center = { sphereB.center.x,sphereB.center.y,sphereB.center.z };

	Vector3 diff = a_center - b_center;
	float distance = diff.length();
	float total_radius = sphereA.radius + sphereB.radius;

	// If the distance between the centers of the spheres is less than the sum of their radii, then they are colliding
	if ( distance < total_radius )
	{
// Calculate the collision depth and direction
		float collision_depth = total_radius - distance;
		Vector3 collision_direction = diff.norm();

		// Output the collision depth multiplied by the collision direction
		out_collision_depth_direction = collision_direction * collision_depth;

		out_collision_depth_direction *= -1;

		out_collision_depth_direction.y = 0;

		return true;
	}

	return false;
}

Vector3 CollisionManager::ResolveCollision(const Sphere& sphereA,const Sphere& sphereB) {
	Vector3 collision_depth_direction;
	if ( DetectCollision(sphereA,sphereB,collision_depth_direction) )
	{

		

	}

	return collision_depth_direction;
}
