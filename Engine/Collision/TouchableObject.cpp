#include "TouchableObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include <CollisionManager.h>


TouchableObject* TouchableObject::Create(Model* model, WorldTransform& worldTrans, unsigned short attribute)
{
	// オブジェクトのインスタンスを生成
	TouchableObject* instance = new TouchableObject();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(model, worldTrans, attribute)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool TouchableObject::Initialize(Model* model, WorldTransform& worldTrans, unsigned short attribute)
{

	// コライダーの追加
	MeshCollider* collider = new MeshCollider();
	// コリジョンマネージャに追加
	collider->ConstructTriangles(model, worldTrans.matWorld_);
	collider->SetAttribute(attribute);
	CollisionManager::GetInstance()->AddCollider(collider);

	return true;
}