#include "stdafx.h"
#include "EnemyManager.h"
#include "WalkEnemy.h"
#include "Player_BulletManager.h"
#include "EnemyBulletManager.h"
#include "GroundingLauncher.h"
#include "FlyEnemy.h"
//#include "GameBase/GameBase.h"

EnemyManager::EnemyManager()
{
}


EnemyManager::~EnemyManager()
{
}

void EnemyManager::Start()
{
	//EnemyBulletManager�̏������B
	G_EnemyBulletManager().Start();
}

void EnemyManager::Update()
{
	for (std::list<Enemy*>::iterator itr = EnemyList.begin(); itr != EnemyList.end(); itr++) {
		G_Player_BulletManager().EnemyHit(*itr);
		G_Player_BulletManager().EnemyHitMine(*itr);
		G_Player_BulletManager().EnemyHitBlackhole(*itr);
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
	Enemy* enemy;
	enemy = NewGO<Enemy>(0);
	enemy->SetPosition(position);
	EnemyList.push_back(enemy);
}

void EnemyManager::spawnGL(CVector3 position)
{
	Enemy* enemy;
	enemy = NewGO<GroundingLauncher>(0);
	enemy->SetPosition(position);
	EnemyList.push_back(enemy);
}

void EnemyManager::spawnFly(CVector3 position)
{
	Enemy* enemy;
	enemy = NewGO<FlyEnemy>(0);
	enemy->SetPosition(position);
	EnemyList.push_back(enemy);
}
void EnemyManager::Deleteenemy()
{
	for (std::list<Enemy*>::iterator itr = EnemyList.begin(); itr != EnemyList.end(); ) {
		if ((*itr)->Getenemy_deathflag() == true) {
			//���S���Ă���΍폜����B
			DeleteGO(*itr);
			itr = EnemyList.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void EnemyManager::AllDeleteEnemy()
{
	for (std::list<Enemy*>::iterator itr = EnemyList.begin(); itr != EnemyList.end(); ) {
		DeleteGO(*itr);
		itr = EnemyList.erase(itr);
	}

	//EnemyBulletManager�̏������B
	G_EnemyBulletManager().OnDestroy();
}