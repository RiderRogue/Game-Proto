#pragma once
#include "Enemy.h"
using namespace YTEngine;

struct EnemyState {
	Enemy* enemy;
	bool Aliveflag;//�G�������Ă��邩�������t���O�B
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
	*@brief	�G�̃X�|�[���B
	*/
	void spawnenemy();

private:
	/*!
	*@brief	�G�̃X�|�[���B
	*/
	void Deleteenemy();
	static const int Enemy_NUM = 20;       //�G�̔z��̐��B
	EnemyState enemy_slot[Enemy_NUM];
};

