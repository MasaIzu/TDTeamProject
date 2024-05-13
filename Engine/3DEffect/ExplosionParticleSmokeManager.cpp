#include "ExplosionParticleSmokeManager.h"


ExplosionParticleSmokeManager::ExplosionParticleSmokeManager()
{

}

ExplosionParticleSmokeManager::~ExplosionParticleSmokeManager()
{

}

void ExplosionParticleSmokeManager::Initialize()
{
	Smoke = std::make_unique<ExplosionParticleSmoke>();
	Smoke->Initialize(MaxParticle);
	Smoke->SetTextureHandle(TextureManager::Load("sprite/smoke1.png"));
}

void ExplosionParticleSmokeManager::CSUpdate(ID3D12GraphicsCommandList* cmdList,const Vector4& Pos)
{
	Smoke->CSUpdate(cmdList,static_cast<uint32_t>(isExp),Pos);
	isExp = false;
}

void ExplosionParticleSmokeManager::Draw(const ViewProjection& view)
{
	ID3D12GraphicsCommandList* commandList = DirectXCore::GetInstance()->GetCommandList();
	ExplosionParticleSmoke::PreDraw(commandList);
	Smoke->Draw(view);
	ExplosionParticleSmoke::PostDraw();
}

void ExplosionParticleSmokeManager::MakeParticle()
{
	isExp = true;
}
