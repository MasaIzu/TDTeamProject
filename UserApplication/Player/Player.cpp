#include "Player.h"
#include "ImGuiManager.h"
#include "Enemy.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "Numbers.h"
#include "PlayerEnum.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(const unsigned short Attribute,ViewProjection* viewProjection)
{
	input_ = Input::GetInstance();
	model_.reset(Model::CreateFromOBJ("cube", true));
	playerFbx_;

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,0 };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.TransferMatrix();

	animation = std::make_unique<Animation>();
	animation->Initialize("katate");

	animation2 = std::make_unique<Animation>();
	animation2->Initialize("3JamJiki");

	viewProjection_ = viewProjection;

	experienceToNextLevel = baseExperience;

	speed = 1.0f;


	// �R���W�����}�l�[�W���ɒǉ�
	float sphereF = 0;
	playerCollider = new SphereCollider(Vector4(sphereF, playerRadius, sphereF, sphereF), playerRadius);
	CollisionManager::GetInstance()->AddCollider(playerCollider);
	Attribute_ = Attribute;
	playerCollider->SetAttribute(Attribute_);

	playerCollider->Update(animation->GetBonePos(0) * worldTransform_.matWorld_);

}

void Player::Update(Input* input)
{
	isBladeAttack = false;

	Move(input);


	if (isBladeAttacking == false)
	{
		if (input_->MouseInputTrigger(static_cast<int>(1)) || input_->ButtonInput(RT))
		{
			isBladeAttack = true;
		}
	}
	AttackUpdate();
	if (isBladeAttack == true)
	{
		PlayerBladeAttack();
	}
	animation2->Update();
	worldTransform_.TransferMatrix();
	CheckHitCollision();
	HpUpdate();
}

void Player::Draw(const ViewProjection& LightViewProjection_)
{
	//model_->Draw(worldTransform_, *viewProjection_, LightViewProjection_);
}

void Player::Move(Input* input)
{
	if (input->PushKey(DIK_W))
	{
		velocity_ = { 0,0,speed };
		worldTransform_.translation_ += velocity_ ;
	}
	if (input->PushKey(DIK_S))
	{
		velocity_ = { 0,0,speed * -1 };
		worldTransform_.translation_ += velocity_;
	}
	if (input->PushKey(DIK_A))
	{
		velocity_ = { speed * -1 ,0,0, };
		worldTransform_.translation_ += velocity_;
	}
	if (input->PushKey(DIK_D))
	{
		velocity_ = { speed ,0,0, };
		worldTransform_.translation_ += velocity_;
	}
	if (input->PushKey(DIK_RETURN))
	{
		AddExperience(1);
	}

	playerCollider->Update(animation->GetBonePos(0) * worldTransform_.matWorld_);

	ImGui::Begin("experience");
	ImGui::SetWindowPos({ 200 , 200 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputInt("expeience", &experience);
	ImGui::InputInt("Nextexpeience", &experienceToNextLevel);
	ImGui::InputInt("level", &level);
	ImGui::End();
}

void Player::FbxDraw(const ViewProjection& lightViewProjection_)
{
	//animation->FbxDraw(worldTransform_, *viewProjection_, lightViewProjection_);
	animation2->FbxDraw(worldTransform_, *viewProjection_, lightViewProjection_);
}

void Player::FbxShadowDraw(const ViewProjection& lightViewProjection_)
{
	animation->FbxShadowDraw(worldTransform_, lightViewProjection_);
}

void Player::CheckHitCollision()
{
	if (playerCollider->GetHit())
	{
		isHit_ = true;
		playerCollider->Reset();//����������������Z�b�g
	}


}

void Player::HpUpdate()
{
	//���i�U����h�����߂̏���
	if (hitCooltime_ <= 0)
	{
		isHit_ = false;
		hitCooltime_ = 5.0f;
	}

	if (isHit_ == true)
	{
		//�q�b�g�̃N�[���^�C���������l��������
		if (hitCooltime_ >= 5.0f)
		{
			hp_ -= 1;
		}

		hitCooltime_--;
	}


	if (hp_ > FloatNumber(fNumbers::fZero))
	{

	}
	else
	{
		hp_ = FloatNumber(fNumbers::fZero);//0�Œ�
		isAlive_ = false;
		playerCollider->SetAttribute(COLLISION_ATTR_INVINCIBLE);
	}
}

void Player::AddExperience(int amount)
{
	experience += amount;
	while (experience >= experienceToNextLevel) {
		LevelUp();
	}
}

void Player::LevelUp()
{
	experience -= experienceToNextLevel;
	level++;
	experienceToNextLevel = CalculateNextLevelExperience();
}

int Player::CalculateNextLevelExperience() const
{
	return static_cast<int>(baseExperience * pow(ratio, level - 1));//���̏ꍇ��1��2��100�o���l�A����ȍ~�͑O�̃��x���A�b�v�ŕK�v�ɂȂ����o���l��1.5�{�K�v�ɂȂ�z��
}

void Player::PlayerBladeAttack()
{
	isBladeAttacking = true;
	isPreparation = false;
	BladeAttackTime = static_cast<uint32_t>(Numbers::Zero);
}

void Player::AttackUpdate()
{
	if (isBladeAttacking == true)
	{
		if (isPreparation == false)
		{
			if (BladeAttackTime < BladeMaxAttackTime)
			{
				BladeAttackTime++;
				
				animation2->SetKeepAnimation(static_cast<uint32_t>(PlayerAnimation::HandAttack), static_cast<uint32_t>(Numbers::Ten), playerAnimTime.BladeAttack);
				if (input_->MouseInputTrigger(static_cast<int>(Numbers::One)))
				{
					BladeAttackTime = BladeMaxAttackTime;
					animation2->SetAnimation(static_cast<uint32_t>(PlayerAnimation::HandAttack), static_cast<uint32_t>(Numbers::Ten), playerAnimTime.BladeAttack, false);
				}
			}
			else
			{
				isPreparation = true;
				animation2->SetAnimation(static_cast<uint32_t>(PlayerAnimation::HandAttack), static_cast<uint32_t>(Numbers::Ten), playerAnimTime.BladeAttack, false);
			}
		}
		else
		{
			if (animation2->GetAnimAlmostOver(BladeColEndHasten))
			{
				isBladeAttacking = false;
				CollisionManager::GetInstance()->ResetMeleeAttack();
			}
		}
	}
}
