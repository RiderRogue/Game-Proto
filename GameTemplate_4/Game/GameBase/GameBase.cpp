#include "stdafx.h"
#include "GameBase.h"
#include "../Player.h"
#include "../Gamecamera.h"
#include "../EnemyManager.h"



GameBase::GameBase()
{
}
GameBase::~GameBase()
{
}

bool GameBase::Start()
{
	/*NewGO<Level>(2, "Level");

	NewGO<Player>(1, "Player");
	level = FindGO<Level>("Level");
	level->Init(L"Assets/level/stage_02.tkl", nullptr);
	NewGO<Gamecamera>(2, "Gamecamera");
	G_EnemyManager().Start();
	G_EnemyManager().spawnenemy({ 100.0f,0.0f,100.0f });*/
	stage_num = 0;
	//タイトル画面
	ChangeScene(GameBase_title);
	return true;
}

void GameBase::Update()
{
	/*static bool startflag = false;
	if (startflag == false) {
		ChangeScene(GameBase_title);
		startflag = true;
	}*/
}

void GameBase::ChangeScene(GameBaseState state)
{
	switch (state)
	{
	case GameBase_title:
		if (b_stageselect != nullptr) {
			DeleteGO(b_stageselect);
			b_stageselect = nullptr;
		}
		if (b_maingame != nullptr) {
			DeleteGO(b_maingame);
			b_maingame = nullptr;
		}
		b_title=NewGO<title>(1, "title");	
		break;

	case GameBase_stageselect:
		if (b_title != nullptr) {
			DeleteGO(b_title);
			b_title = nullptr;
		}
		if (b_maingame != nullptr) {
			DeleteGO(b_maingame);
			b_maingame = nullptr;
		}
		//ステージ番号の初期化。
		stage_num = 0;
		//3Dカメラの初期化。
		g_camera3D.SetPosition({ 0.0f, 200.0f, 700.0f });
		g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
		b_stageselect = NewGO<Stageselect>(1, "Stageselect");
		break;

	case GameBase_mainGame:
		if (b_stageselect != nullptr) {
			DeleteGO(b_stageselect);
			b_stageselect = nullptr;
		}
		if (b_title != nullptr) {
			DeleteGO(b_title);
			b_title = nullptr;
		}
		b_maingame = NewGO<MainGame>(2, "MainGame");
		break;
	}
}