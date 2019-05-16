#include "stdafx.h"
#include "EnemyManager.h"
#include "WalkEnemy.h"
#include "Player_BulletManager.h"
#include "EnemyBulletManager.h"
//#include "GameBase/GameBase.h"

EnemyManager::EnemyManager()
{
}


EnemyManager::~EnemyManager()
{
}

void EnemyManager::Start()
{
	for (int i = 0; i < Enemy_NUM; i++) {
		//flag��false�ɂ��āA��������Ă��Ȃ��悤�ɏ���������B
		enemy_slot[i].Aliveflag = false;
	}
	//EnemyBulletManager�̏������B
	G_EnemyBulletManager().Start();
}

void EnemyManager::Update()
{
	for (int i = 0; i < Enemy_NUM; i++) {
		//�����Ă���z�̂ݓ����蔻����Ƃ�B
		if (enemy_slot[i].Aliveflag == true) {
			G_Player_BulletManager().EnemyHit(enemy_slot[i].enemy);
			
		}
	}
	G_EnemyBulletManager().Update();
	//���S����B
	Deleteenemy();
}

void EnemyManager::EnemyBulletDraw()
{
	G_EnemyBulletManager().Draw();
}

void EnemyManager::spawnenemy(CVector3 position)
{
	for (int i = 0; i < Enemy_NUM; i++) {
		if (enemy_slot[i].Aliveflag == false) {
			//�G�I�u�W�F�N�g�𐶐�&�����t���O���グ�Ă����B
			enemy_slot[i].enemy = NewGO<Enemy>(0);
			enemy_slot[i].enemy->SetPosition(position);
			enemy_slot[i].enemy->SetslotNumber(i);
			enemy_slot[i].Aliveflag = true;
			break;
		}
	}
}

void EnemyManager::Deleteenemy()
{
	for (int i = 0; i < Enemy_NUM; i++) {
		//Aliveflag��true�̓G�̒����玀��ł���G��T���B
		if (enemy_slot[i].Aliveflag == true) {
			if (enemy_slot[i].enemy->Getenemy_deathflag() == true) {
				//�G�I�u�W�F�N�g���폜�B
				DeleteGO(enemy_slot[i].enemy);
				enemy_slot[i].Aliveflag = false;
			}
		}
	}
}

void EnemyManager::AllDeleteEnemy()
{
	for (int i = 0; i < Enemy_NUM; i++) {
		//�����Ă���G�𒲂ׂ�B
		if (enemy_slot[i].Aliveflag == true) {
			//�G�I�u�W�F�N�g���폜�B
			DeleteGO(enemy_slot[i].enemy);
			enemy_slot[i].Aliveflag = false;
		}
	}
	//EnemyBulletManager�̏������B
	G_EnemyBulletManager().OnDestroy();
}