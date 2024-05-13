#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <vector>
MY_SUPPRESS_WARNINGS_END

class AddShield
{
private:
	struct Shield
	{
		Vector3 pos;
		Vector3 scale = { 1,1,1 };
	};

public://基本関数
	AddShield();
	~AddShield();

	//初期化
	void Initialize();
	//更新
	void Update(const Vector3& colPos,const Vector3& colScale);
	//描画
	void Draw(const ViewProjection& ViewProjection_,const ViewProjection& LightViewProjection_);
	void ShadowDraw(const ViewProjection& lightViewProjection_);

	void SetFin();

private://メンバ関数

public://Setter

public://Getter

private://クラス関連

private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数

	bool isChange = false;

	uint32_t count = 0;

	std::unique_ptr<Model> ShildModel;
	std::vector<uint32_t> colCount;
	std::vector<WorldTransform> ShieldsTrans;

};