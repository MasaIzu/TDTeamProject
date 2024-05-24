#include "PlayerAnimation.h"


PlayerAnimation::PlayerAnimation()
{
}

PlayerAnimation::~PlayerAnimation()
{
}

void PlayerAnimation::Initialize()
{
	AnmNumNumber_ = static_cast<uint32_t>(0);
	//fbx
	fbxModel_.reset(FbxLoader::GetInstance()->LoadModelFromFile("3JamJiki", true));
	fbxObj3d_ = FBXObject3d::Create();
	fbxObj3d_->SetModel(fbxModel_.get());
	fbxObj3d_->PlayAnimation(AnmNumNumber_);
	fbxObj3d_->Update();

}

void PlayerAnimation::Update()
{
	if (isStart)
	{
		if (nowAnmFCount_ < maxFcount)
		{
			isMotionFinish = false;
			nowAnmFCount_++;
		}
		else
		{
			isStart = false;
			isMotionFinish = true;
			if (isLoop)
			{
				nowAnmFCount_ = static_cast<uint32_t>(0);
				isStart = true;
			}
		}
	}

	fbxObj3d_->Update(AnmNumNumber_, nowAnmFCount_, maxFcount);
}

void PlayerAnimation::FbxDraw(const WorldTransform& worldTransform, const ViewProjection& viewProjection_, const ViewProjection& lightViewProjection_)
{
	fbxObj3d_->Draw(worldTransform, viewProjection_, lightViewProjection_);
}

void PlayerAnimation::FbxShadowDraw(const WorldTransform& worldTransform, const ViewProjection& lightViewProjection_)
{
	fbxObj3d_->ShadowDraw(worldTransform, lightViewProjection_);
}

void PlayerAnimation::SetAnimation(const uint32_t& animNumber, const uint32_t& startCount, const uint32_t& maxCount, const bool& loop)
{
	if (isStart == false)
	{
		nowAnmFCount_ = startCount;
	}
	isStart = true;
	isMotionFinish = false;
	isLoop = loop;
	AnmNumNumber_ = animNumber;
	maxFcount = maxCount;
}

void PlayerAnimation::SetKeepAnimation(const uint32_t& animNumber, const uint32_t& Count, const uint32_t& maxCount)
{
	isStart = false;
	isMotionFinish = false;
	AnmNumNumber_ = animNumber;
	nowAnmFCount_ = Count;
	maxFcount = maxCount;
}

bool PlayerAnimation::GetNowAnimFinish()
{
	return isMotionFinish;
}

bool PlayerAnimation::GetAnimAlmostOver(const float& count)
{
	if (nowAnmFCount_ >= (maxFcount - count)) {
		return true;
	}
	return false;
}

Matrix4 PlayerAnimation::GetBonePos(const uint32_t& boneNum)
{
	return fbxObj3d_->GetBonesMatPtr(boneNum);
}