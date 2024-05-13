#pragma once


#include "Vector3.h"

#include "WorldTransform.h"

#include "TouchableObject.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <map>
#include <string>
#include <vector>
#include <Model.h>
MY_SUPPRESS_WARNINGS_END

// レベルデータ
struct LevelData {

	struct ObjectData {
		// ファイル名
		std::string fileName;
		// 平行移動
		Vector3 translation;
		// 回転角
		Vector3 rotation;
		// スケーリング
		Vector3 scaling;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects;
};

struct ModelData {
	Model* model;
	WorldTransform worldTrans;
	std::string ModelName = "";
};

// レベルデータの生成
class LoadLevelEditor {

public:// 定数
	// デフォルトの読み込みディレクトリ
	static const std::string kDefaultBaseDirectory;
	// ファイル拡張子
	static const std::string kExtension;

public:// メンバ関数

	LoadLevelEditor();
	~LoadLevelEditor();

	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	LevelData* LoadFile(const std::string& fileName,const Vector3& vec3);

	//初期化
	void Initialize(const std::string& fileName ,const Vector3& vec3);
	//更新
	void Update();
	//更新
	void CSUpdate(ID3D12GraphicsCommandList* cmdList,const bool& isNotBlack = true);
	//描画
	void Draw(const ViewProjection& viewProjection,const ViewProjection& LightViewProjection_);
	//描画
	void ShadowDraw(const ViewProjection& viewProjection);
	//描画
	void ParticleDraw(const ViewProjection& viewProjection);

private:
	std::unique_ptr<LevelData> levelData;
	std::map<std::string, Model*> models;
	std::vector<ModelData> LoadedObjects;
	std::vector<ModelData> NewLoadObjects;

	//最初から読み込むやつ
	std::unique_ptr<Model> modelSrop;
	std::unique_ptr<Model> modelGround;
	std::unique_ptr<Model> modelGoal;
	std::unique_ptr<Model> modelNormalEnemy;
	std::unique_ptr<Model> modelBulletShotEnemy;
	std::unique_ptr<Model> torchModel;

	std::vector<Vector3> BillPos;


};