#include "LightData.h"

LightData* LightData::LightData_ = nullptr;
LightGroup* LightData::lightGroup = nullptr;

LightData* LightData::GetInstance() {
	if ( LightData_ == nullptr )
	{
		LightData_ = new LightData();
	}

	return LightData_;
}

void LightData::Destroy() {

	delete LightData_;
	delete lightGroup;
}

void LightData::ClearLight()
{
	lightGroup->ClearLight();
}

void LightData::StaticInitialize()
{
	lightGroup = new LightGroup;
	lightGroup->Initialize();
}

void LightData::Update()
{
	lightGroup->Update();
}

LightData::~LightData()
{

}

uint32_t LightData::AddPointLight(const Vector3& lightpos_,const Vector3& lightcolor_,const Vector3& lightAtten_,const bool& active_)
{
	PointLight pointLight;
	pointLight.SetLightPos(lightpos_);
	pointLight.SetLightColor(lightcolor_);
	pointLight.SetLightAtten(lightAtten_);
	pointLight.SetActive(active_);

	return lightGroup->SetPointLight(pointLight);
}

uint32_t LightData::AddSpotLight(const Vector3& lightpos_,const Vector4& lightdir_,const Vector3& lightcolor_,const Vector3& lightAtten_,const bool& active_)
{
	SpotLight spotLight;
	spotLight.SetLightPos(lightpos_);
	spotLight.SetLightDir(lightdir_);
	spotLight.SetLightColor(lightcolor_);
	spotLight.SetLightAtten(lightAtten_);
	spotLight.SetActive(active_);

	return lightGroup->SetSpotLight(spotLight);
}

uint32_t LightData::AddCircleShadow(const Vector3& casterPos_,const float& distanceCasterLight, const Vector4& dir_,const Vector3& atten_,const Vector2& factorAngle_,const bool& active_)
{
	CircleShadow circleShadow;
	circleShadow.SetCasterPos(casterPos_);
	circleShadow.SetDistanceCasterLight(distanceCasterLight);
	circleShadow.SetDir(dir_);
	circleShadow.SetAtten(atten_);
	circleShadow.SetFactorAngle(factorAngle_);
	circleShadow.SetActive(active_);

	return lightGroup->SetCircleShadow(circleShadow);
}

void LightData::UpdatePointLight(const uint32_t& LightNumbar,const Vector3& lightpos_,const Vector3& lightcolor_,const Vector3& lightAtten_,const bool& active_)
{
	PointLight pointLight;
	pointLight.SetLightPos(lightpos_);
	pointLight.SetLightColor(lightcolor_);
	pointLight.SetLightAtten(lightAtten_);
	pointLight.SetActive(active_);

	lightGroup->SetPointLightUpdate(pointLight,LightNumbar);
}

void LightData::UpdateSpotLight(const uint32_t& LightNumbar,const Vector3& lightpos_,const Vector4& lightdir_,const Vector3& lightcolor_,const Vector3& lightAtten_,const bool& active_)
{
	SpotLight spotLight;
	spotLight.SetLightPos(lightpos_);
	spotLight.SetLightDir(lightdir_);
	spotLight.SetLightColor(lightcolor_);
	spotLight.SetLightAtten(lightAtten_);
	spotLight.SetActive(active_);

	lightGroup->SetSpotLightUpdate(spotLight,LightNumbar);
}

void LightData::UpdateCircleShadow(const uint32_t& ShadowNumbar,const Vector3& casterPos_,const float& distanceCasterLight,const Vector4& dir_,const Vector3& atten_,const Vector2& factorAngle_,const bool& active_)
{
	CircleShadow circleShadow;
	circleShadow.SetCasterPos(casterPos_);
	circleShadow.SetDistanceCasterLight(distanceCasterLight);
	circleShadow.SetDir(dir_);
	circleShadow.SetAtten(atten_);
	circleShadow.SetFactorAngle(factorAngle_);
	circleShadow.SetActive(active_);

	lightGroup->SetCircleShadowUpdate(circleShadow,ShadowNumbar);
}

void LightData::SetAmbientColor(const float& AmbientColor)
{
	lightGroup->SetAmbientColor(AmbientColor);
}

LightGroup* LightData::GetLightGroupData()
{
	return lightGroup;
}

