#include "stdafx.h"
#include "../Player.h"
#include "GameBase.h"
#include "../Gamecamera.h"



GameBase::GameBase()
{
}


GameBase::~GameBase()
{
	DeleteGO(FindGO<Gamecamera>("Gamecamera"));
	DeleteGO(FindGO<Player>("Player"));
	DeleteGO(le);
}

bool GameBase::Start()
{
	
	NewGO<Level>(2, "Level");
	NewGO<Player>(1,"Player");
	le = FindGO<Level>("Level");
	le->Init(L"Assets/level/stage_02.tkl", nullptr);
	NewGO<Gamecamera>(2, "Gamecamera");
	
	return true;
}

void GameBase::Update()
{
}