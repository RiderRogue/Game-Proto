#pragma once
#include "Enemy.h"
using namespace YTEngine;

struct EnemyState {
	Enemy* enemy;
	bool Aliveflag;//敵が生きているかを示すフラグ。
};
class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();
	void Start();
	void Update();
	/*!
	*@brief	敵のスポーン。
	*/
	void spawnenemy(CVector3 position);
	/*!
	*@brief	EnemyManagerインスタンスの取得。
	*/
	static EnemyManager& Instance()
	{
		static EnemyManager instance;//EnemyManagerのインスタンスを生成。
		return instance;
	}
	/*!
	*@brief	敵の全削除。ゲーム終了時の初期化に使う。
	*/
	void AllDeleteEnemy();
private:
	/*!
	*@brief	敵の死亡処理。
	*/
	void Deleteenemy();
	static const int Enemy_NUM = 5;       //敵の配列の数。
	EnemyState enemy_slot[Enemy_NUM];
};

//EnemyManagerのインスタンスを取得。
static inline EnemyManager& G_EnemyManager()
{
	return EnemyManager::Instance();
}
