#include "CSVLoader.h"

CSVLoader* CSVLoader::csvLoader = nullptr;

CSVLoader::CSVLoader()
{
}

CSVLoader* CSVLoader::GetInstance()
{
	if (csvLoader == nullptr)
	{
		csvLoader = new CSVLoader();
	}
	return csvLoader;
}

void CSVLoader::LoadCSVfile(const std::string& fileNames)
{
	//�t�@�C�����J��
	std::string kDefaultTextureDirectoryPath = "Resources/ParticleCSVFile/";
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
