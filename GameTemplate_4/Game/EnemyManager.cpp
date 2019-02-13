#include "stdafx.h"
#include "EnemyManager.h"


EnemyManager::EnemyManager()
{
}


EnemyManager::~EnemyManager()
{
}

bool EnemyManager::Start()
{
	for (int i = 0; i < Enemy_NUM; i++) {
		//flagをfalseにして、発射されていないように初期化する。
		enemy_slot[i].Aliveflag = false;
	}
	return true;
}

void EnemyManager::Update()
{
	Deleteenemy();
}

void EnemyManager::spawnenemy()
{
	for (int i = 0; i < Enemy_NUM; i++) {
		if (enemy_slot[i].Aliveflag == false) {
			//敵オブジェクトを生成&生存フラグを上げておく。
			enemy_slot[i].enemy = NewGO<Enemy>(0);
			enemy_slot[i].Aliveflag = true;
		}
	}
}

void EnemyManager::Deleteenemy()
{
	for (int i = 0; i < Enemy_NUM; i++) {
		if (enemy_slot[i].enemy->Getenemy_deathflag() == true) {
			//敵オブジェクトを削除。
			DeleteGO(enemy_slot[i].enemy);
			enemy_slot[i].Aliveflag = false;
		}
	}
}
void EnemyManager::Draw()
{

}