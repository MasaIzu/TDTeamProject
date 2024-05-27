#include "Player.h"
#include "ImGuiManager.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(ViewProjection* viewProjection)
{

	model_.reset(Model::CreateFromOBJ("cube", true));
	playerFbx_;

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 50,0,50 };
	worldTransform_.scale_ = { 10.0f,3.0f,4.0f };
	worldTransform_.TransferMatrix();

	playerSword = new PlayerSword();
	playerSword->Initialize();

	playerSword->SetParent(&worldTransform_);

	/*playerSword->GetWorldTransform().parent_ = worldTransform_;*/

	//worldTransform_.parent_ = playerSword->GetParent();

	viewProjection_ = viewProjection;

	experienceToNextLevel = baseExperience;
}

void Player::Update(Input* input)
{
	Move(input);
	worldTransform_.TransferMatrix();
	playerSword->Update();
}

void Player::Draw(const ViewProjection& LightViewProjection_)
{
	model_->Draw(worldTransform_, *viewProjection_, LightViewProjection_);
	playerSword->Draw(*viewProjection_, LightViewProjection_);
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

	ImGui::Begin("experience");
	ImGui::SetWindowPos({ 200 , 200 });
	ImGui::SetWindowSize({ 500,100 });
	ImGui::InputInt("expeience", &experience);
	ImGui::InputInt("Nextexpeience", &experienceToNextLevel);
	ImGui::InputInt("level", &level);
	ImGui::End();
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
	return static_cast<int>(baseExperience * pow(ratio, level - 1));//この場合は1→2は100経験値、それ以降は前のレベルアップで必要になった経験値の1.5倍必要になる想定
}
