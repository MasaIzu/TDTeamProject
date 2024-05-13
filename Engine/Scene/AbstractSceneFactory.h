#pragma once
#include "BaseScene.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <string>
MY_SUPPRESS_WARNINGS_END

/// <summary>
/// シーンファクトリー
/// </summary>
class AbstractSceneFactory
{
public:
	// 仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	// シーン生成
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};