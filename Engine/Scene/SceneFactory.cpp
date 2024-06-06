#include "SceneFactory.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "DebugScene.h"


BaseScene* SceneFactory::CreateScene(const std::string& sceneName,int score)
{
	// 次のシーンを生成
	BaseScene* newScene = nullptr;

	if ( sceneName == "TITLE" )
	{
		newScene = new TitleScene(score);
	}
	else if ( sceneName == "SELECT" )
	{
		newScene = new SelectScene(score);
	}
	else if ( sceneName == "GAMEPLAY" )
	{
		newScene = new GameScene();
	}
	else if ( sceneName == "Debug" )
	{
		newScene = new DebugScene();
	}


	return newScene;
}
