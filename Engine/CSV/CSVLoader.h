#pragma once
#include <memory>
#include <list>
#include <cassert>
#include <sstream>
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>

class CSVLoader
{
public:
	//ƒVƒ“ƒOƒ‹ƒgƒ“
	static CSVLoader* GetInstance();

	void LoadCSVfile(const std::string& fileNames);

	void UpdateCSVfile();

	void ResetCSVfile();

private:
	CSVLoader() = default;
	~CSVLoader();
	CSVLoader(const CSVLoader&) = delete;
	const CSVLoader& operator=(const CSVLoader&) = delete;

	static CSVLoader* csvLoader;


private:
	std::stringstream enemyCSV;
};