#include "LoadLevelEditor.h"
#include "ParticleEditor.h"

MY_SUPPRESS_WARNINGS_BEGIN
#include "External/json.hpp"
#include <fstream>
#include <cassert>
MY_SUPPRESS_WARNINGS_END

const std::string LoadLevelEditor::kDefaultBaseDirectory = "Resources/levels/";
const std::string LoadLevelEditor::kExtension = ".json";

LoadLevelEditor::LoadLevelEditor()
{
}

LoadLevelEditor::~LoadLevelEditor()
{

	for (auto& object : NewLoadObjects) {
		if (object.model) {
			delete object.model;
		}
	}
	//for (Ground* Allground : ground) {
	//	delete Allground;
	//}

}

LevelData* LoadLevelEditor::LoadFile(const std::string& fileName,const Vector3& vec3) {
	// 連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelDataLocal = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();
		//std::string addType = object["file_name"].get<std::string>();
		if (object["file_name"].get<std::string>().empty()) {
			// MESH
			if (type.compare("MESH") == 0) {
				// 要素追加
				levelDataLocal->objects.emplace_back(LevelData::ObjectData{});
				// 今追加した要素の参照を得る
				LevelData::ObjectData& objectData = levelDataLocal->objects.back();

				if (object.contains("name")) {
					// ファイル名
					objectData.fileName = object["name"];
				}

				// トランスフォームのパラメータ読み込み
				nlohmann::json& transform = object["transform"];
				// 平行移動
				objectData.translation.x = static_cast<float>(transform["translation"][0]) + vec3.x;
				objectData.translation.y = static_cast<float>(transform["translation"][2]) + vec3.y;
				objectData.translation.z = static_cast<float>(transform["translation"][1] + vec3.z);

				// 回転角
				objectData.rotation.x = static_cast<float>(transform["rotation"][1]);
				objectData.rotation.y = static_cast<float>(transform["rotation"][2]);
				objectData.rotation.z = static_cast<float>(transform["rotation"][0]);
				// スケーリング
				objectData.scaling.x = static_cast<float>(transform["scaling"][1]);
				objectData.scaling.y = static_cast<float>(transform["scaling"][2]);
				objectData.scaling.z = static_cast<float>(transform["scaling"][0]);

				// TODO: コライダーのパラメータ読み込み
			}
		}
		
		// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
		if (object.contains("children")) {

		}
	}

	return levelDataLocal;
}


void LoadLevelEditor::Initialize(const std::string& FileName,const Vector3& vec3)
{

	levelData.reset(LoadFile(FileName,vec3));

	// モデル読み込み
	modelSrop.reset(Model::CreateFromOBJ("srop1", true));
	modelGround.reset(Model::CreateFromOBJ("stageTest", true));
	modelGoal.reset(Model::CreateFromOBJ("goal", true));
	//modelFighter = Model::CreateFromOBJ("chr_sword", true);
	modelNormalEnemy.reset(Model::CreateFromOBJ("sphereNormalEnemy", true));
	modelBulletShotEnemy.reset(Model::CreateFromOBJ("sphereBulletEnemy", true));
	torchModel.reset(Model::CreateFromOBJ("torch",true));

	models.insert(std::make_pair("srop1", modelSrop.get()));
	models.insert(std::make_pair("stageTest", modelGround.get()));
	models.insert(std::make_pair("goal", modelGoal.get()));
	//models.insert(std::make_pair("chr_sword", modelFighter));
	models.insert(std::make_pair("sphereNormalEnemy", modelNormalEnemy.get()));
	models.insert(std::make_pair("sphereBulletEnemy", modelBulletShotEnemy.get()));
	models.insert(std::make_pair("torch",torchModel.get()));

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {

		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;

		std::string fileName;

		if (objectData.fileName.find(".") != std::string::npos) {
			for (uint32_t i = 0; i < objectData.fileName.length(); i++) {
				if (objectData.fileName.substr(i, 1) != ".") {
					fileName += objectData.fileName.substr(i, 1);
				}
				else {
					break;
				}
			}
		}
		else {
			fileName = objectData.fileName;
		}

		decltype(models)::iterator it = models.find(fileName);
		if (it != models.end()) {
			model = it->second;
			// 座標
			WorldTransform Trans;
			Trans.Initialize();
			Trans.translation_ = objectData.translation;// 座標
			Trans.scale_ = objectData.scaling;

			Trans.SetRot(MyMath::GetAngleVec3(objectData.rotation));// 回転角
			Trans.TransferMatrix();
		}
		else {
			// モデルを指定して3Dオブジェクトを生成
			model = Model::CreateFromOBJ(fileName, true);
			// 座標
			WorldTransform Trans;
			Trans.Initialize();
			Trans.translation_ = objectData.translation;
			Trans.scale_ = objectData.scaling;// 座標
			Trans.SetRot(objectData.rotation);// 回転角

			// 配列に登録
			ModelData Data;
			Data.model = model;
			Data.worldTrans = Trans;
			Vector3 Scale_ = Data.worldTrans.scale_;
			Data.worldTrans.scale_ = Vector3(Scale_.z,Scale_.y,Scale_.x);
			Data.worldTrans.TransferMatrix();
			Data.ModelName = fileName;
			NewLoadObjects.push_back(Data);

			if (fileName == "cube") {
				TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_LANDSHAPE);
			}
			else if (fileName == "shahei") {
				TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_LANDSHAPE);
			}
			else if ( fileName == "jimen" )
			{
				TouchableObject::Create(model,Data.worldTrans,COLLISION_ATTR_LANDSHAPE);
				Vector3 Scale = Data.worldTrans.scale_ * 100 ;
				Vector3 Trans_ = MyMath::GetWorldTransform(Data.worldTrans.matWorld_) + Vector3(0,-Scale.y,0);
				ParticleEditor::AddCollision(Trans_,Scale);
			}
			else if ( fileName == "Manshon" )
			{
				TouchableObject::Create(model,Data.worldTrans,COLLISION_ATTR_LANDSHAPE);
				Vector3 Scale = Data.worldTrans.scale_;
				Vector3 Trans_ = MyMath::GetWorldTransform(Data.worldTrans.matWorld_) + Vector3(0,Scale.y,0);
				ParticleEditor::AddCollision(Trans_,Scale);
				BillPos.push_back(Data.worldTrans.translation_);
			}
			else if (fileName == "goalSrop") {
				//TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_FINALRAIL);
			}
			else if (fileName == "foundationCube") {
				TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_LANDSHAPE);
			}
			else if (fileName == "stageSrop") {
				TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_LANDSHAPE);
			}
			else if (fileName == "tutorialSrop") {
				//TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_FIRSTRAIL);
			}
			else if (fileName == "stage2") {
				TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_LANDSHAPE);
			}
			else if (fileName == "stage2Srop") {
				//TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_RAIL);
			}
			else if (fileName == "stageSrop2") {
				TouchableObject::Create(model, Data.worldTrans, COLLISION_ATTR_LANDSHAPE);
			}
		}

	}
	//for (Ground* AllGround : ground) {
	//	AllGround->Initialze();
	//}

	Update();
}

void LoadLevelEditor::Update()
{
	//for (auto& modelTrans : LoadedObjects) {
	//	modelTrans.worldTrans.TransferMatrix();
	//}
	for (auto& modelTrans : NewLoadObjects) {
		modelTrans.worldTrans.TransferMatrix();
	}
}

void LoadLevelEditor::CSUpdate(ID3D12GraphicsCommandList* cmdList,const bool& isNotBlack)
{

}

void LoadLevelEditor::Draw(const ViewProjection& viewProjection,const ViewProjection& LightViewProjection_)
{

}

void LoadLevelEditor::ShadowDraw(const ViewProjection& viewProjection)
{

}

void LoadLevelEditor::ParticleDraw(const ViewProjection& viewProjection)
{

}
