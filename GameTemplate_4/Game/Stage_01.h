#pragma once
#include "Level.h"
class Stage_01 : public IGameObject
{
public:
	Stage_01();
	~Stage_01();
	bool Start();
	void Update();
	void Draw();
	void OnDestroy();
private:
	Level * level;
	SkinModel m_model;//ãÛÅB
	std::vector<CVector3>FlyEnemylist;
};

