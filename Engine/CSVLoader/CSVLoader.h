#pragma once
#include <memory>
#include <list>
#include <cassert>
#include <sstream>
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include "CSVLoader.h"

class CSVLoader
{
public:

	//モデルの番号
	int enemyModelNmb;
	//経験値量
	float expPoint;
	//攻撃力
	float attackPoint;
	//敵の行動
	int enemyAction;
	//速度
	float enemyMoveSpeed;
public:

	CSVLoader();
	~CSVLoader();

	//static CSVLoader* GetInstance();



	void LoadCSVFile(const std::string& fileNames);


private:

	void LoadEnemyCSVfile(const std::string& fileNames);

	void  UpdateCSVfile();

	void ResetCSVfile();
	//CSVLoader() = default;
	//~CSVLoader();
	//CSVLoader(const CSVLoader&) = delete;
	//const CSVLoader& operator=(const CSVLoader&) = delete;

	//static CSVLoader* csvLoader;


private:
	std::stringstream enemyCSV;

	bool waitflag = false;
	int waitTimer = 120;
	//EnemyParameter* parameter;
};