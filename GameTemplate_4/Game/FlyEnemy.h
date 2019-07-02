#pragma once
#include "Enemy.h"
class FlyEnemy :
	public Enemy
{
public:
	FlyEnemy();
	~FlyEnemy();
	bool Start();
	void Update();
};

