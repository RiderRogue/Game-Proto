#pragma once
using namespace YTEngine;
#include "Level.h"
#include "gauge.h"
class MainGame : public IGameObject
{
public:
	MainGame();
	~MainGame();
	bool Start();
	void Update();
	void PostDraw();
	void OnDestroy();
private:
	Level * level;
	float s_x=1.0f;
	gauge* m_gauge;
};

