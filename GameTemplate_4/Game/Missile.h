#pragma once
/*!
* @brief	ミサイル。
*/
//#include "Enemy.h"
#include "character/BulletController.h"
using namespace YTEngine;
class Enemy;
class Missile : public IGameObject
{
public:
	Missile();
	~Missile();
	bool Start();
	void Update();
	void Draw();
private:
	bool flag;//弾丸が発射中かどうか
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //スキンモデル。
	BulletController m_bulletCon;    //弾丸の剛体。
	int lengthcount;                   //弾丸の移動距離。
	bool desflag;                      //生存しているか？
	Enemy* lockon_enemy;          //ロックオンした敵。
};

