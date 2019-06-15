#pragma once
using namespace YTEngine;
#include "Level.h"
#include "EnemyBoss_Launcher.h"

class Stage_03 : public IGameObject
{
public:
	Stage_03();
	~Stage_03();
	bool Start();
	void Update();
	void Draw();
	void OnDestroy();
private:
	Level * level;
	SkinModel m_model;//ãÛÅB
	CVector3 E_PosR;
	CVector3 E_PosL;
	CVector3 E_PosFront;
	CVector3 E_PosBack;
	EnemyBoss_Launcher* boss;
};

