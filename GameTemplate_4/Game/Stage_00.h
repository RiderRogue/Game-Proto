#pragma once
using namespace YTEngine;
#include "Level.h"
//#include "Stage_Base.h"
class Stage_00 : public IGameObject
{
public:
	Stage_00();
	~Stage_00();
	bool Start();
	void Update();
	void Draw();
	void OnDestroy();
private:
	Level * level;
	SkinModel m_model;//ãÛÅB
	std::vector<CVector3>FlyEnemylist;
};

