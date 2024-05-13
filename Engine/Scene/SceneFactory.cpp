#include "SceneFactory.h"
#include "GameScene.h"
#include "DebugScene.h"


BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    // 次のシーンを生成
    BaseScene* newScene = nullptr;

    if (sceneName == "TITLE") {
        newScene = new GameScene();
    }
    else if (sceneName == "GAMEPLAY") {
        newScene = new GameScene();
    }
    else if (sceneName == "Debug") {
        newScene = new DebugScene();
    }


    return newScene;
}
