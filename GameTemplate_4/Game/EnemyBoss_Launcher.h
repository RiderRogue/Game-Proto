#pragma once
using namespace YTEngine;

#include "Enemy.h"
#include "Physics/CollisionAttr.h"
class EnemyBoss_Launcher : public Enemy
{
public:
	EnemyBoss_Launcher();
	~EnemyBoss_Launcher();
	bool Start();
	void Update();
	void Draw();
	void OnDestroy();
	void PostDraw();

	void SetPosition(CVector3 pos)
	{
		m_position = pos;
		m_position.y += 0.0f;
		//現在はカプセル
		m_charaCon.Init(
			enemy_weight,
			enemy_height,
			m_position
		);
		m_charaCon.GetRigidBody()->GetBody()->setUserIndex(enCollisionAttr_Enemy);
	}

	void SetActionRange(CVector3 R, CVector3 L, CVector3 F, CVector3 B)
	{
		E_PosR = R;
		E_PosL = L;
		E_PosFront = F;
		E_PosBack = B;
	}
	void Normalmove();
	//回転撃ちの処理。
	void RotateShot();

	void enemyDamage_Blackhole(int damage, CVector3 b_position);
private:
	CVector3 E_PosR;
	CVector3 E_PosL;
	CVector3 E_PosFront;
	CVector3 E_PosBack;
	CVector3 R_LauncherPos;
	CVector3 L_LauncherPos;

	CVector3 MoveVector;//予め保存しておく移動ベクトル。
	bool FrontSaid;
	int ActionCount = 0;
	enum AttackNum {
		move,
		rotShot,
		Attack_Sum    //合計
	};
	enum AttackNum attack;
};

