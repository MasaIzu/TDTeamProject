#include "CSVLoader.h"

//CSVLoader* CSVLoader::csvLoader = nullptr;

CSVLoader::CSVLoader()
{
}
//
//CSVLoader* CSVLoader::GetInstance()
//{
//	if (csvLoader == nullptr)
//	{
//		csvLoader = new CSVLoader();
//	}
//	return csvLoader;
//}

void CSVLoader::LoadCSVFile(const std::string& fileNames)
{
	LoadEnemyCSVfile(fileNames);

	UpdateCSVfile();

	ResetCSVfile();
}

void CSVLoader::LoadEnemyCSVfile(const std::string& fileNames)
{
	//�t�@�C�����J��
	std::string kDefaultTextureDirectoryPath = "Resources/EnemyCSVFile/";
	std::string CSVpath = ".csv";
	std::ifstream file;
	std::string fullPath = kDefaultTextureDirectoryPath + fileNames + CSVpath;
	file.open(fullPath);
	assert(file.is_open());

	//�t�@�C������e�𕶎���X�g���[���ɃR�s�[
	enemyCSV << file.rdbuf();

	//�t�@�C�������
	file.close();
}

void CSVLoader::UpdateCSVfile()
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
	while (getline(enemyCSV, line))
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

		//���f���̔ԍ��ǂݍ���
		if (word.find("ModelNmb") == 0)
		{
			//���f���̔ԍ�
			getline(line_stream, word, ',');
			int x = (float)std::atof(word.c_str());
			enemyModelNmb = x;
		}
		//�o���l�ʓǂݍ���
		if (word.find("ExpPoint") == 0)
		{
			//�o���l��
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			expPoint = x;
		}
		//�U���͓ǂݍ���
		if (word.find("AttackPoint") == 0)
		{
			//�U����
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			attackPoint = x;
		}
		//�s���ǂݍ���
		if (word.find("Action") == 0)
		{
			//�s���ԍ�
			getline(line_stream, word, ',');
			int x = (float)std::atof(word.c_str());
			enemyModelNmb = x;
		}
		//�����ǂݍ���
		if (word.find("MoveSpeed") == 0)
		{
			//����
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			enemyMoveSpeed = x;
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

void CSVLoader::ResetCSVfile()
{
	enemyCSV.str("");
	enemyCSV.clear(std::stringstream::goodbit);
}

CSVLoader::~CSVLoader()
{
}