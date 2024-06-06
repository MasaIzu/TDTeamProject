#pragma once
#include "Model.h"
#include <map>
#include <string>

class ModelManager
{
public:
	//シングルトンインスタンス
	static ModelManager* GetInstance();
	/// <summary>
	/// モデルファイルの読み込み
	/// </summary>
	/// <param name="filePath">モデルファイルのパス</param>
	void LoadModel(const std::string& filePath);
	/// <summary>
	/// モデルの検索(OBJ)
	/// </summary>
	/// <param name="filePath">モデルのファイルパス</param>
	/// <returns>モデル</returns>
	static Model* FindObjModel(const std::string& filePath);


	//終了
	void Finalize();

private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	const ModelManager& operator=(const ModelManager&) = delete;

	static ModelManager* modelManager;
private:

	//モデルデータ
	static std::map<std::string,std::unique_ptr<Model>> models;

};