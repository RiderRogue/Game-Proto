#pragma once
#include "Level.h"
using namespace YTEngine;
class GameBase : public IGameObject
{
public:
	GameBase();
	~GameBase();
	bool Start();
	void Update();
private:
	Level* le;
};

