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

	//���f���̔ԍ�
	int enemyModelNmb;
	//�o���l��
	float expPoint;
	//�U����
	float attackPoint;
	//�G�̍s��
	int enemyAction;
	//���x
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