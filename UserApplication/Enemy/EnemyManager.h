#pragma once
#include "Vector3.h"
#include "Enemy.h"

#include <memory>
#include <list>
#include <cassert>
#include "ViewProjection.h"

class Player;
///<summary>
///雑魚敵とボスの生成のまとめ
///</summary>
class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void Initialize(ViewProjection* viewProjection,Player* player, const unsigned short Attribute);

	///<summary>
	////更新
	///</summary>
	void Update();
	///<summary>
	////描画
	///</summary>
	void Draw(const ViewProjection& LightViewProjection_);

	///<summary>
	////プレイヤーのセット
	///</summary>
	void SetPlayer(Player* player) { player_ = player; };


	/// <summary>
	/// 敵のCSVの読み込み
	/// </summary>
	void LoadEnemyPopData();

	//CSVリセット
	void EnemyPopComandReset();

	/// <summary>
	/// 敵のCSVの解析
	/// </summary>
	void UpdateEnemyPopCommands();

	//敵の登録
	void ExistenceEnemy(const Vector3& EnemyPos);

	//フォルダ以下のファイル一覧を取得する関数
	//folderPath  フォルダパス
	//file_names  ファイル名一覧
	//return        true:成功,false : 失敗
	bool getFileNames(std::string folderPath, std::vector<std::string>& file_names);
private:

	Player* player_ = nullptr;

	bool waitflag = false;
	int waitTimer = 0;

	int popTime = 0;
	int popResTime = 120;

	std::list<std::unique_ptr<Enemy>> enemy_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	ViewProjection* view;

	std::unique_ptr<Model> model;

	int enemySpornCount = 2;

#pragma region
	unsigned short Attribute_;


#pragma endregion
};