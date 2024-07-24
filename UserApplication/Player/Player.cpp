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

void Player::Initialize(const unsigned short Attribute, ViewProjection* viewProjection)
{
	input_ = Input::GetInstance();
	model_.reset(Model::CreateFromOBJ("sphere", true));
	playerFbx_;

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,0 };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.TransferMatrix();

	animation = std::make_unique<Animation>();
	animation->Initialize("katate");

	animation2 = std::make_unique<Animation>();
	animation2->Initialize("3JamJiki");

	playerMovement = std::make_unique<PlayerMovement>();

	viewProjection_ = viewProjection;

	experienceToNextLevel = baseExperience;

	speed = 1.0f;

	// コリジョンマネージャに追加
	float sphereF = 0;
	playerCollider = new SphereCollider(Vector4(sphereF, playerRadius, sphereF, sphereF), playerRadius);
	CollisionManager::GetInstance()->AddCollider(playerCollider);
	Attribute_ = Attribute;
	playerCollider->SetAttribute(Attribute_);

	playerCollider->Update(animation2->GetBonePos(0) * worldTransform_.matWorld_);

	for (uint32_t i = 0; i < AttackColSphereCount; i++)
	{
		BladeColWorldTrans[i].scale_ = Vector3(PlayerBladeRadius, PlayerBladeRadius, PlayerBladeRadius);
		BladeColWorldTrans[i].Initialize();
		PlayerBladeAttackCollider[i] = new SphereCollider(Vector4(sphereF, PlayerBladeRadius, sphereF, sphereF), PlayerBladeRadius);
		CollisionManager::GetInstance()->AddCollider(PlayerBladeAttackCollider[i]);
		PlayerBladeAttackCollider[i]->SetAttribute(COLLISION_ATTR_NOTATTACK);
		PlayerBladeAttackCollider[i]->Update(worldTransform_.matWorld_);
	}

	int MaxParticleCountB = 20000;
	particleEditor = std::make_unique<ParticleEditor>();
	particleEditor->Initialize(MaxParticleCountB, true, "HonooBlade");
	particleEditor->SetTextureHandle(TextureManager::Load("sprite/effect4.png"));

	trail3D_ = std::make_unique<Trail3D>(50);
	trail3D_->SetTexture(TextureManager::Load("sprite/gomi.png"));

	SunderRail.push_back(Vector3(0,80,0));
	SunderRail.push_back(Vector3(0,60,0));
	SunderRail.push_back(Vector3(0,40,0));
	SunderRail.push_back(Vector3(0,20,0));
	SunderRail.push_back(Vector3(0,0,0));

	LoadPlayerStatusData();
	UpdatePlayerStatusData();
}

void Player::Update(Input* input)
{
	isBladeAttack = false;
	isHit_ = false;

	if ( playerCollider->GetHitSphere() )
	{
		isHit_ = true;
		playerCollider->ResetSphere();
	}


	Move(input);


	if (isBladeAttacking == false)
	{
		if (input_->MouseInputTrigger(static_cast<int>(1)) || input_->ButtonInput(RT))
		{
			isBladeAttack = true;
		}
	}

	isLeftAttack = false;

	if ( isLeftAttacking == false )
	{
		if ( input_->MouseInputTrigger(static_cast< int >( 0 )) || input_->ButtonInput(RT) )
		{
			isLeftAttack = true;
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

	PlayerRot();

	playerStateNeedMaterial.worldTransform = worldTransform_;
	playerStateNeedMaterial.isAlive = true;
	playerStateNeedMaterial.onGround = true;
	playerStateNeedMaterial.isBladeAttack = isBladeAttacking;

	worldTransform_.translation_ += playerMovement->Move(playerStateNeedMaterial);

	ParticleStartPos = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(animation2->GetBonePos(RightBoneNum) * worldTransform_.matWorld_));
	ParticleEndPos = MyMath::Vec3ToVec4(MyMath::GetWorldTransform(animation2->GetBonePos(BladeAttackEndPos) * worldTransform_.matWorld_));

	BladeColRatio = MyMath::Vec4ToVec3(ParticleEndPos) - MyMath::Vec4ToVec3(ParticleStartPos);
	ParticleMilEndPos = ParticleStartPos + MyMath::Vec3ToVec4(BladeColRatio.norm() * MaxBladeColDetection);
	BladeColRatio = (BladeColRatio.norm() * MaxBladeColDetection) / AttackColSphereCount;

	for (uint32_t i = 0; i < AttackColSphereCount; i++)
	{
		BladeColWorldTrans[i].translation_ = MyMath::Vec4ToVec3(ParticleStartPos) + (BladeColRatio * static_cast<float>(i));
		BladeColWorldTrans[i].TransferMatrix();
		PlayerBladeAttackCollider[i]->Update(BladeColWorldTrans[i].matWorld_);
	}

}

void Player::Draw(const ViewProjection& LightViewProjection_)
{
	//model_->Draw(worldTransform_, *viewProjection_, LightViewProjection_);

	//for (uint32_t i = 0; i < AttackColSphereCount; i++)
	//{
	//	model_->Draw(BladeColWorldTrans[i], *viewProjection_, LightViewProjection_);
	//}
}

void Player::TarilDraw()
{
	trail3D_->Draw(*viewProjection_);
}

void Player::Move(Input* input)
{
	//if (input->PushKey(DIK_W))
	//{
	//	velocity_ = { 0,0,speed };
	//	worldTransform_.translation_ += velocity_ ;
	//}
	//if (input->PushKey(DIK_S))
	//{
	//	velocity_ = { 0,0,speed * -1 };
	//	worldTransform_.translation_ += velocity_;
	//}
	//if (input->PushKey(DIK_A))
	//{
	//	velocity_ = { speed * -1 ,0,0, };
	//	worldTransform_.translation_ += velocity_;
	//}
	//if (input->PushKey(DIK_D))
	//{
	//	velocity_ = { speed ,0,0, };
	//	worldTransform_.translation_ += velocity_;
	//}
	//if (input->PushKey(DIK_RETURN))
	//{
	//	AddExperience(1);
	//}


	if (playerCollider->GetHit())
	{
		score_ -= addscore_;
	}

	playerCollider->Update(animation2->GetBonePos(static_cast< uint32_t >( Numbers::Three )) * worldTransform_.matWorld_);

	//ImGui::Begin("experience");
	//ImGui::SetWindowPos({ 200 , 200 });
	//ImGui::SetWindowSize({ 500,100 });
	////ImGui::InputInt("expeience", &experience);
	////ImGui::InputInt("Nextexpeience", &experienceToNextLevel);
	////ImGui::InputInt("level", &level);
	//ImGui::InputInt("score_",&score_);
	//ImGui::End();
}

void Player::CSUpdate(ID3D12GraphicsCommandList* cmdList)
{
	particleEditor->CSUpdate(cmdList, ParticleStartPos, ParticleEndPos, static_cast<uint32_t>(isBladeAttacking));
}

void Player::ParticleDraw()
{
	particleEditor->Draw(*viewProjection_);
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
		playerCollider->Reset();//当たった判定をリセット
	}


}

void Player::HpUpdate()
{
	//多段攻撃を防ぐための処理
	if (hitCooltime_ <= 0)
	{
		isHit_ = false;
		hitCooltime_ = 5.0f;
	}

	if (isHit_ == true)
	{
		//ヒットのクールタイムが初期値だったら
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
		hp_ = FloatNumber(fNumbers::fZero);//0固定
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
	return static_cast<int>(baseExperience * pow(ratio, level - 1));//この場合は1→2は100経験値、それ以降は前のレベルアップで必要になった経験値の1.5倍必要になる想定
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
				BladeAttributeSet(COLLISION_ATTR_MELEEATTACK);
			}
		}
		else
		{
			if (animation2->GetAnimAlmostOver(BladeColEndHasten))
			{
				isBladeAttacking = false;
				BladeAttributeSet(COLLISION_ATTR_NOTATTACK);
				CollisionManager::GetInstance()->ResetMeleeAttack();
			}
		}
	}
	Vector3 SetPos;
	//trail3D_->SetPos(SetPos);
	trail3D_->Update();

	if ( isLeftAttack )
	{
		SunderTopPos = EnemyPos + UpPos;
		SunderBottomPos = EnemyPos;
		trail3D_->ResetTrail(SunderRail[0]);

		for ( int i = 0; i < 5; i++ )
		{
			SetPos = SunderRail[ i ];
			trail3D_->SetPos(SetPos);
			trail3D_->SetIsVisible(true);
			trail3D_->Update();
		}

	}

}

void Player::BladeAttributeSet(const unsigned short Attribute_)
{
	for (auto&& col : PlayerBladeAttackCollider)
	{
		col->SetAttribute(Attribute_);
	}
}

void Player::PlayerRot()
{
	playerMovement->PlayerAngle();
	RotKeep = Vec3Number(fNumbers::fZero);

	RotKeep = Vector3(FloatNumber(fNumbers::fZero), MyMath::GetAngle(playerMovement->GetPlayerAngle()), FloatNumber(fNumbers::fZero));
	playerRotWorldTrans.SetRot(RotKeep);
	worldTransform_.SetRot(Vector3(FloatNumber(fNumbers::fZero), MyMath::GetAngle(playerMovement->GetPlayerAngle()), FloatNumber(fNumbers::fZero)));


	//値更新
	WorldTransUpdate();
}

void Player::WorldTransUpdate()
{
	worldTransform_.TransferMatrix();
}

void Player::GetEnemyPos(const Vector3& enemyPos)
{
	EnemyPos = enemyPos;
}

bool Player::getFileNames(std::string folderPath,std::vector<std::string>& file_names)
{
	using namespace std::filesystem;
	directory_iterator iter(folderPath),end;
	std::error_code err;

	for ( ; iter != end && !err; iter.increment(err) )
	{
		const directory_entry entry = *iter;

		file_names.push_back(entry.path().string());
		printf("%s\n",file_names.back().c_str());
	}

	/* エラー処理 */
	if ( err )
	{
		std::cout << err.value() << std::endl;
		std::cout << err.message() << std::endl;
		return false;
	}
	return true;
}

void Player::LoadPlayerStatusData()
{
	//ファイルを開く
	std::ifstream file;
	////敵の出現CSVの名前
	//std::vector<std::string> CSVFileNames;
	//getFileNames("Resources/CsvFile/PlayerData/playerStatus.csv",CSVFileNames);

	file.open("Resources/CsvFile/PlayerData/playerStatus.csv");

	assert(file.is_open());

	//ファイルを内容を文字列ストリームにコピー
	playerStatusCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void Player::UpdatePlayerStatusData()
{
	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(playerStatusCommands,line) )
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream,word,',');

		//"//"から始まる行はコメント
		if ( word.find("//") == 0 )
		{
			//コメント行は飛ばす
			continue;
		}
		//POPコマンド
		if ( word.find("POWER") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			int power = ( int ) std::atoi(word.c_str());
			SetPower(power);
		}

		//HPコマンド
		else if ( word.find("HP") == 0 )
		{
			//x座標
			getline(line_stream,word,',');
			int hp = ( int ) std::atoi(word.c_str());
			SetHp(hp);
			break;
		}

	}
}
