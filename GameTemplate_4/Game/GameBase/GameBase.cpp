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
	//ƒ^ƒCƒgƒ‹‰æ–Ê
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
		if (b_maingame != nullptr) {
			DeleteGO(b_maingame);
		}
		b_title = NewGO<title>(1, "title");
		break;

	case GameBase_mainGame:
		if (b_title != nullptr) {
			DeleteGO(b_title);
		}
		b_maingame = NewGO<MainGame>(2, "MainGame");
		break;
	}
}