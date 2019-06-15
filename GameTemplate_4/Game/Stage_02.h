#pragma once
using namespace YTEngine;
#include "Level.h"
class Stage_02 : public IGameObject
{
public:
	Stage_02();
	~Stage_02();
	bool Start();
	void Update();
	void Draw();
	void OnDestroy();
private:
	Level * level;
	SkinModel m_model;//ãÛÅB
	std::vector<CVector3>FlyEnemylist;
};

