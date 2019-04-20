#include "stdafx.h"
#include "MainGame.h"
#include "Player.h"
#include "Gamecamera.h"
#include "EnemyManager.h"
#include "GameBase/GameBase.h"

MainGame::MainGame()
{
}


MainGame::~MainGame()
{
	
}

void MainGame::OnDestroy()
{
	DeleteGO(FindGO<Gamecamera>("Gamecamera"));
	DeleteGO(FindGO<Player>("Player"));
	DeleteGO(level);
	G_EnemyManager().AllDeleteEnemy();
}

bool MainGame::Start()
{
	NewGO<Level>(2, "Level");
	
	NewGO<Player>(1, "Player");
	level = FindGO<Level>("Level");
	level->Init(L"Assets/level/level01_proto02.tkl", nullptr);
	NewGO<Gamecamera>(2, "Gamecamera");
	G_EnemyManager().Start();
	G_EnemyManager().spawnenemy({ 100.0f,0.0f,100.0f });
	G_EnemyManager().spawnenemy({ 200.0f,0.0f,180.0f });
	
	return true;
}

void MainGame::Update()
{
	
	G_EnemyManager().Update();

	if (g_pad[0].IsPress(enButtonB))
	{
		FindGO<GameBase>("GameBase")->ChangeScene(GameBase::GameBase_title);
	}
	s_x -= 0.01f;
}

void MainGame::PostDraw()
{
	
}