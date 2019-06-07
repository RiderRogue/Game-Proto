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
	g_camera3D.SetPosition({ 0.0f, 500.0f, 800.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(50000.0f);
	
	int gameObjectPrioMax = 32;
	g_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	g_camera2D.SetWidth(FRAME_BUFFER_W);
	g_camera2D.SetHeight(FRAME_BUFFER_H);
	g_camera2D.SetPosition({ 0.0f, 0.0f, -10.0f });
	g_camera2D.SetTarget(CVector3::Zero());
	g_camera2D.Update();

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