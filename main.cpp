#include <Windows.h>
#include <memory>

#include "MainGame.h"
#include "Framework.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	std::unique_ptr<Framework> myGame = std::make_unique<MainGame>();


	myGame->Run();
    
	return 0;
}