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
	//ファイルを開く
	std::string kDefaultTextureDirectoryPath = "Resources/EnemyCSVFile/";
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

	if (waitflag)
	{
		waitTimer--;
		if (waitTimer <= 0)
		{
			//待機完了
			waitflag = false;
		}
		return;
	}
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

		//モデルの番号読み込み
		if (word.find("ModelNmb") == 0)
		{
			//モデルの番号
			getline(line_stream, word, ',');
			int x = (float)std::atof(word.c_str());
			enemyModelNmb = x;
		}
		//経験値量読み込み
		if (word.find("ExpPoint") == 0)
		{
			//経験値量
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			expPoint = x;
		}
		//攻撃力読み込み
		if (word.find("AttackPoint") == 0)
		{
			//攻撃力
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			attackPoint = x;
		}
		//行動読み込み
		if (word.find("Action") == 0)
		{
			//行動番号
			getline(line_stream, word, ',');
			int x = (float)std::atof(word.c_str());
			enemyModelNmb = x;
		}
		//速さ読み込み
		if (word.find("MoveSpeed") == 0)
		{
			//速さ
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			enemyMoveSpeed = x;
		}
		//WAITコマンド
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			waitflag = true;
			waitTimer = waitTime;

			//コマンドループを抜ける
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