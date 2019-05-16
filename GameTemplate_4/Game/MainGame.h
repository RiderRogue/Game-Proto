#pragma once
using namespace YTEngine;
#include "Level.h"

class MainGame : public IGameObject
{
public:
	MainGame();
	~MainGame();
	bool Start();
	void Update();
	void Draw();
	void OnDestroy();
private:
	Level * level;
	float s_x=1.0f;
	SkinModel m_model;
};

