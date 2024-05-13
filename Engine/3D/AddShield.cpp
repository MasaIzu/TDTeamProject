#include "AddShield.h"
#include <ParticleEditor.h>

AddShield::AddShield()
{
	ShildModel.reset(Model::CreateFromOBJ("Manshon",true));
}

AddShield::~AddShield()
{
	isChange = true;
}

void AddShield::Initialize()
{
	WorldTransform shieldPos;
	shieldPos.Initialize();
	ShieldsTrans.push_back(shieldPos);

	colCount.push_back(ParticleEditor::AddCollision(shieldPos.translation_,shieldPos.scale_));

	count = static_cast< uint32_t >( colCount.size() - 1 );

	isChange = true;
}

void AddShield::Update(const Vector3& colPos,const Vector3& colScale)
{
	if ( isChange )
	{
		ShieldsTrans[ count ].translation_ = colPos;
		ShieldsTrans[ count ].scale_ = colScale;
		ShieldsTrans[ count ].TransferMatrix();
		Vector3 Col = colPos + Vector3(0,colScale.y,0);
		ParticleEditor::ChangeCollision(colCount[ count ],Col,colScale);
	}
}

void AddShield::Draw(const ViewProjection& ViewProjection_,const ViewProjection& LightViewProjection_)
{
	for ( auto&& trans : ShieldsTrans )
	{
		ShildModel->Draw(trans,ViewProjection_,LightViewProjection_);
	}
}

void AddShield::ShadowDraw(const ViewProjection& lightViewProjection_)
{
	for ( auto&& trans : ShieldsTrans )
	{
		ShildModel->ShadowDraw(trans,lightViewProjection_);
	}
}

void AddShield::SetFin()
{
	isChange = false;
}

