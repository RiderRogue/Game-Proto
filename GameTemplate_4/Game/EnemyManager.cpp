#include "stdafx.h"
#include "EnemyManager.h"

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
		//flagをfalseにして、生成されていないように初期化する。
		enemy_slot[i].Aliveflag = false;
	}
}

void EnemyManager::Update()
{
	for (int i = 0; i < Enemy_NUM; i++) {
		//生きている奴のみ
		if (enemy_slot[i].Aliveflag == true) {
			bool bullethitflag = G_Player_BulletManager().EnemyHit(enemy_slot[i].enemy->GetPosition_center());
			if (bullethitflag == true) {
				//被弾していれば
				//10引く。
				enemy_slot[i].enemy->enemyDamage(10);
			}
		}
	}

	//当たり判定。
	Deleteenemy();
}

void EnemyManager::spawnenemy(CVector3 position)
{
	for (int i = 0; i < Enemy_NUM; i++) {
		if (enemy_slot[i].Aliveflag == false) {
			//敵オブジェクトを生成&生存フラグを上げておく。
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
		//Aliveflagがtrueの敵の中から死んでいる敵を探す。
		if (enemy_slot[i].Aliveflag == true) {
			if (enemy_slot[i].enemy->Getenemy_deathflag() == true) {
				//敵オブジェクトを削除。
				DeleteGO(enemy_slot[i].enemy);
				enemy_slot[i].Aliveflag = false;
			}
		}
	}
}

void EnemyManager::AllDeleteEnemy()
{
	for (int i = 0; i < Enemy_NUM; i++) {
		//生きている敵を調べる。
		if (enemy_slot[i].Aliveflag == true) {
			//敵オブジェクトを削除。
			DeleteGO(enemy_slot[i].enemy);
			enemy_slot[i].Aliveflag = false;
		}
	}

}