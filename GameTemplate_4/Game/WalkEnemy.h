#pragma once
using namespace YTEngine;
#include"Enemy.h"
class WalkEnemy : public Enemy
{
public:
	WalkEnemy();
	~WalkEnemy();
	bool Start();
	void Update();
};

