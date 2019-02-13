#pragma once
#include "Enemy.h"
using namespace YTEngine;

struct EnemyState {
	Enemy* enemy;
	bool Aliveflag;//敵が生きているかを示すフラグ。
};
class EnemyManager : public IGameObject
{
public:
	EnemyManager();
	~EnemyManager();
	bool Start();
	void Update();
	void Draw();
	/*!
	*@brief	敵のスポーン。
	*/
	void spawnenemy();

private:
	/*!
	*@brief	敵のスポーン。
	*/
	void Deleteenemy();
	static const int Enemy_NUM = 20;       //敵の配列の数。
	EnemyState enemy_slot[Enemy_NUM];
};

