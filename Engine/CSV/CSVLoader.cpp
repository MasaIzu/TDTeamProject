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
	//ファイルを開く
	std::string kDefaultTextureDirectoryPath = "Resources/ParticleCSVFile/";
	std::string CSVpath = ".csv";
	std::ifstream file;
	std::string fullPath = kDefaultTextureDirectoryPath + fileNames + CSVpath;
	file.open(fullPath);
	assert(file.is_open());

	//ファイルを内容を文字列ストリームにコピー
	enemyCSV << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void CSVLoader::UpdateCSVfile()
{
	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyCSV, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0)
		{
			//コメント行は飛ばす
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
