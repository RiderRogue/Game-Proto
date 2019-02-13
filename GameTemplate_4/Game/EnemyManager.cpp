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
		//flag��false�ɂ��āA���˂���Ă��Ȃ��悤�ɏ���������B
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
			//�G�I�u�W�F�N�g�𐶐�&�����t���O���グ�Ă����B
			enemy_slot[i].enemy = NewGO<Enemy>(0);
			enemy_slot[i].Aliveflag = true;
		}
	}
}

void EnemyManager::Deleteenemy()
{
	for (int i = 0; i < Enemy_NUM; i++) {
		if (enemy_slot[i].enemy->Getenemy_deathflag() == true) {
			//�G�I�u�W�F�N�g���폜�B
			DeleteGO(enemy_slot[i].enemy);
			enemy_slot[i].Aliveflag = false;
		}
	}
}
void EnemyManager::Draw()
{

}