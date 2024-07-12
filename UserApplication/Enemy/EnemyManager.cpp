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
	popTime--;
	enemy_.remove_if([](std::unique_ptr<Enemy>& enemy)
		{
			return enemy->IsDead();
		});

	for (std::unique_ptr<Enemy>& enemy : enemy_)
	{
		enemy->Update();
	}
	if (popTime<=0) {
		EnemyPopComandReset();
		popTime = popResTime;
	}
	UpdateEnemyPopCommands();


}

void EnemyManager::Draw(const ViewProjection& LightViewProjection_)
{
	for (std::unique_ptr<Enemy>& enemy : enemy_)
	{
		//敵キャラの描画
		enemy->Draw(LightViewProjection_);
	}
}

void EnemyManager::LoadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	//敵の出現CSVの名前
	std::vector<std::string> CSVFileNames;
	getFileNames("Resources/EnemyCSVFile", CSVFileNames);
	int fileNmb = 0;
	for (int i = 0; i < CSVFileNames.size(); i++)
	{
		fileNmb++;
	}
	int randCSV = rand() % fileNmb;

	file.open(CSVFileNames[0]);

	assert(file.is_open());

	//ファイルを内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
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
			//待機完了
			waitflag = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line))
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
		}
		//POPコマンド
		if (word.find("POP") == 0)
		{
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			//敵を発生させる
			ExistenceEnemy(Vector3(x, y, z));
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

void EnemyManager::ExistenceEnemy(const Vector3& EnemyPos)
{
	//敵キャラの生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(view,EnemyPos,0,player_, Attribute_,power_,hp_);

	//リストに登録する
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

	/* エラー処理 */
	if (err)
	{
		std::cout << err.value() << std::endl;
		std::cout << err.message() << std::endl;
		return false;
	}
	return true;
}
