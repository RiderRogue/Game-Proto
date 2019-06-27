#pragma once
using namespace YTEngine;
#include "Level.h"
#include "Player.h"
class MainGame : public IGameObject
{
public:
	MainGame();
	~MainGame();
	bool Start();
	void Update();
	void Draw();
	void OnDestroy();

	//プレイヤーの死亡フラグをあげる。
	void Des_Player() {
		Player_desFlag = true;
	}
private:
	Player * player;
	Level * level;
	SkinModel m_model;
	std::vector<CVector3>FlyEnemylist;
	bool Player_desFlag = false;
	int stage_num;
};

