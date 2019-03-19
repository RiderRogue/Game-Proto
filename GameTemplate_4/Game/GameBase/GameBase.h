#pragma once
using namespace YTEngine;
#include "Level.h"
#include "../title.h"
#include "../MainGame.h"
class GameBase : public IGameObject
{
public:
	enum GameBaseState
	{
		GameBase_title,
		GameBase_mainGame
	};
	GameBase();
	~GameBase();
	bool Start();
	void Update();
	void ChangeScene(GameBaseState state);
private:
	Level* level;
	title* b_title = nullptr;
	MainGame* b_maingame = nullptr;
};
