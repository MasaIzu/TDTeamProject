#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize(ViewProjection* viewProjection,Player* player, const unsigned short Attribute)
{
	view = viewProjection;
	player_ = player; 
	Attribute_ = Attribute;
}

void EnemyManager::Update()
{
	enemy_.remove_if([](std::unique_ptr<Enemy>& enemy)
		{
			return enemy->IsDead();
		});

	for (std::unique_ptr<Enemy>& enemy : enemy_)
	{
		enemy->Update();
	}
	if (Input::GetInstance()->PushKey(DIK_P)) {
		EnemyPopComandReset();
		//LoadEnemyPopData();
	}
	UpdateEnemyPopCommands();
}

void EnemyManager::Draw(const ViewProjection& LightViewProjection_)
{
	for (std::unique_ptr<Enemy>& enemy : enemy_)
	{
		//�G�L�����̕`��
		enemy->Draw(LightViewProjection_);
	}
}

void EnemyManager::LoadEnemyPopData()
{
	//�t�@�C�����J��
	std::ifstream file;
	//�G�̏o��CSV�̖��O
	std::vector<std::string> CSVFileNames;
	getFileNames("Resources/EnemyCSVFile", CSVFileNames);
	int fileNmb = 0;
	for (int i = 0; i < CSVFileNames.size(); i++)
	{
		fileNmb++;
	}
	int randCSV = rand() % fileNmb;

	file.open(CSVFileNames[randCSV]);

	assert(file.is_open());

	//�t�@�C������e�𕶎���X�g���[���ɃR�s�[
	enemyPopCommands << file.rdbuf();

	//�t�@�C�������
	file.close();
}

void EnemyManager::EnemyPopComandReset()
{
	enemyPopCommands.str("");
	enemyPopCommands.clear(std::stringstream::goodbit);
	LoadEnemyPopData();
}

void EnemyManager::UpdateEnemyPopCommands()
{
	if (waitflag)
	{
		waitTimer--;
		if (waitTimer <= 0)
		{
			//�ҋ@����
			waitflag = false;
		}
		return;
	}

	//1�s���̕����������ϐ�
	std::string line;

	//�R�}���h���s���[�v
	while (getline(enemyPopCommands, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		std::string word;
		//,��؂�ōs�̐擪��������擾
		getline(line_stream, word, ',');

		//"//"����n�܂�s�̓R�����g
		if (word.find("//") == 0)
		{
			//�R�����g�s�͔�΂�
			continue;
		}
		//POP�R�}���h
		if (word.find("POP") == 0)
		{
			//x���W
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//y���W
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//z���W
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			//�G�𔭐�������
			ExistenceEnemy(Vector3(x, y, z));
		}

		//WAIT�R�}���h
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			//�҂�����
			int32_t waitTime = atoi(word.c_str());

			//�ҋ@�J�n
			waitflag = true;
			waitTimer = waitTime;

			//�R�}���h���[�v�𔲂���
			break;
		}

	}
}

void EnemyManager::ExistenceEnemy(const Vector3& EnemyPos)
{
	//�G�L�����̐���
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(view,EnemyPos,0,player_, Attribute_);

	//���X�g�ɓo�^����
	enemy_.push_back(std::move(newEnemy));
}

bool EnemyManager::getFileNames(std::string folderPath, std::vector<std::string>& file_names)
{
	using namespace std::filesystem;
	directory_iterator iter(folderPath), end;
	std::error_code err;

	for (; iter != end && !err; iter.increment(err))
	{
		const directory_entry entry = *iter;

		file_names.push_back(entry.path().string());
		printf("%s\n", file_names.back().c_str());
	}

	/* �G���[���� */
	if (err)
	{
		std::cout << err.value() << std::endl;
		std::cout << err.message() << std::endl;
		return false;
	}
	return true;
}
