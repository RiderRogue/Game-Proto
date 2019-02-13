#include "stdafx.h"
#include "system.h"
#include "Player.h"
#include "Level.h"
#include "GameBase/GameBase.h"

using namespace YTEngine;
///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");
	/*Level ka;
	ka.Init(L"Assets/level/stage_02.tkl", nullptr);*/
	//カメラを初期化。
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
	
	int gameObjectPrioMax = 32;
	

	//ゲームループ。
	//エンジンを作成し、初期化。
	if (Engine().Init(gameObjectPrioMax) == true) {
		//GameBaseを作成。
		NewGO<GameBase>(2, "GameBase");

		//ゲームループを実行。
		Engine().RunGameLoop();
	}
	//エンジンの終了処理。
	Engine().Final();
}