#include "ModelManager.h"
ModelManager* ModelManager::modelManager = nullptr;
std::map<std::string,std::unique_ptr<Model>> ModelManager::models;



ModelManager* ModelManager::GetInstance()
{
	if ( modelManager == nullptr )
	{
		modelManager = new ModelManager();
	}
	return modelManager;
}

void ModelManager::LoadModel(const std::string& filePath)
{
	//読み込み済みモデルを検索
	if ( models.contains(filePath) )
	{
		//読み込み済みなら早期return
		return;
	}

	std::unique_ptr<Model> objModel;
	objModel.reset(Model::CreateFromOBJ(filePath,true));

	models.insert(std::make_pair(filePath,std::move(objModel)));
}

Model* ModelManager::FindObjModel(const std::string& filePath)
{
	//読み込み済みモデルを検索
	if ( models.contains(filePath) )
	{
		//読み込みモデルを戻り値としてreturn
		return models.at(filePath).get();
	}

	//ファイル名一致なし
	return nullptr;
}

void ModelManager::Finalize()
{
	models.clear();
	delete modelManager;
}
