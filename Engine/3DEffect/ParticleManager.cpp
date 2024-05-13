#include "ParticleManager.h"
#include "ParticleCS.h"
#include "ParticleHandHanabi.h"
#include "ExplosionParticleSmoke.h"
#include "MeshParticle.h"
#include "ParticleBoost.h"
#include "BulletSmokeParticle.h"
#include "ParticleHanabiExplosion.h"
#include "EnemyBoostParticle.h"
#include "Explosion.h"
#include "ParticleEditor.h"

void ParticleManager::Initialize(ID3D12Device* device)
{
	//ParticleCS::StaticInitialize(device);
	ParticleHandHanabi::StaticInitialize(device);
	ParticleBoost::StaticInitialize(device);
	ExplosionParticleSmoke::StaticInitialize(device);
	BulletSmokeParticle::StaticInitialize(device);
	ParticleHanabiExplosion::StaticInitialize(device);
	EnemyBoostParticle::StaticInitialize(device);
	Explosion::StaticInitialize(device);
	ParticleEditor::StaticInitialize(device);
	//MeshParticle::StaticInitialize(device);
}

void ParticleManager::Finalize()
{
	//MeshParticle::StaticFinalize();
	//Explosion::StaticFinalize();
	ParticleBoost::StaticFinalize();
	ParticleHandHanabi::StaticFinalize();
	//ParticleCS::StaticFinalize();
}
