#include "stdafx.h"
#include "EnemyBoss_Launcher.h"
#include "Player_BulletManager.h"
#include "EnemyBulletManager.h"


EnemyBoss_Launcher::EnemyBoss_Launcher()
{
}


EnemyBoss_Launcher::~EnemyBoss_Launcher()
{
}

void EnemyBoss_Launcher::OnDestroy()
{

}

bool EnemyBoss_Launcher::Start()
{
	ActionCount = 0;
	attack = move;//0なら攻撃無し。
	FrontSaid = true;
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/EnemyBoss.cmo");
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
	m_forward.Normalize();
	//右方向
	m_rite.x = mRot.m[0][0];
	m_rite.y = mRot.m[0][1];
	m_rite.z = mRot.m[0][2];
	m_rite.Normalize();
	
	MoveVector = m_rite;
	//敵弾の半径。
	float radius = 10.0f;
	m_charaCon.Init_bullet(
		radius,
		m_position
	);

	player = FindGO<Player>("Player");
	//回転。
	qBias.SetRotationDeg(CVector3::AxisY(), 180.0f);
	m_rotation.Multiply(qBias);
	m_position = m_charaCon.EnemyExecute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//移動。
	m_position_center = m_charaCon.Getbullet_position();

	R_LauncherPos = m_position_center + (m_forward*400.0f) + (m_rite*400.0f);
	L_LauncherPos = m_position_center + (m_forward*400.0f) + (m_rite*-400.0f);
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
	return true;
}

void EnemyBoss_Launcher::Update()
{
	
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
	m_forward.Normalize();
	//右方向
	m_rite.x = mRot.m[0][0];
	m_rite.y = mRot.m[0][1];
	m_rite.z = mRot.m[0][2];
	m_rite.Normalize();

	G_Player_BulletManager().EnemyHit(this);
	G_Player_BulletManager().EnemyHitMine(this);
	G_Player_BulletManager().EnemyHitBlackhole(this);
	

	
	R_LauncherPos = m_position_center + (m_forward*400.0f) + (m_rite*400.0f);
	L_LauncherPos = m_position_center + (m_forward*400.0f) + (m_rite*-400.0f);
	switch (attack)
	{
	case move:
		Normalmove();
		break;
	case rotShot:
		RotateShot();
		break;
	default:
		Normalmove();
		break;
	}
	
	
	m_position = m_charaCon.EnemyExecute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//移動。
	m_position_center = m_charaCon.Getbullet_position();

	
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
}

void EnemyBoss_Launcher::enemyDamage_Blackhole(int damage, CVector3 b_position)
{
	//ブラックホールの吸収範囲かを調べる。
	CVector3 vDist;
	vDist.Subtract(b_position, m_position_center);
	//ブラックホールの中心に来たら
	if (vDist.Length() <= 60.0f) {
		//受けたダメージ分、HPから引く。
		enemy_HP -= damage;
		if (enemy_HP <= 0) {
			enemy_deathflag = true;
		}
	}

	vDist.Normalize();//正規化。
	m_premoveSpeed = vDist * (800.0f);

}
void EnemyBoss_Launcher::Draw()
{
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void EnemyBoss_Launcher::PostDraw()
{

}

void EnemyBoss_Launcher::Normalmove()
{
	static float move = -2.0f;
	static float distTmp;
	CVector3 vDist;
	vDist.Subtract(E_PosFront, m_position);

	distTmp = vDist.Length();
	
	if (distTmp > 250) {
		m_moveSpeed = MoveVector * (distTmp/move);
	}
	else {
		m_moveSpeed = CVector3::Zero();
		ActionCount++;
	}

	CVector3 nextmove = m_position + m_moveSpeed;
	if ((nextmove.x>E_PosR.x) || (nextmove.x < E_PosL.x)) {
		move *= -1.0f;
	}

	if (ActionCount >= 100) {
		attack = rotShot;
		ActionCount = 0;
	}
	

	
	static int bulletcount = 0;
	if (bulletcount == 0) {
		G_EnemyBulletManager().bulletShot(R_LauncherPos, m_forward);
		G_EnemyBulletManager().bulletShot(L_LauncherPos, m_forward);
		bulletcount++;
	}
	else {
		bulletcount++;
		if (bulletcount >= 1) {
			bulletcount = 0;
		}
	}
	
	
}
void EnemyBoss_Launcher::RotateShot()
{
	static int count = 0;
	static int bulletcount = 0;
	static float rot_sum = 0.0f;
	static float rot = 3.0f;
	static float b_move = -1.0f;

	if (bulletcount == 0) {
		G_EnemyBulletManager().MissileShot(R_LauncherPos, m_forward);
		G_EnemyBulletManager().MissileShot(L_LauncherPos, m_forward);
		//G_EnemyBulletManager().MissileShot(R_LauncherPos, m_forward);
		//G_EnemyBulletManager().MissileShot(L_LauncherPos, m_forward);
		bulletcount++;
	}
	else {
		bulletcount++;
		if (bulletcount >= 60) {
			bulletcount = 0;
		}
	}
	//6セットで終了。
	if (count < 6) {
		//横移動。
		m_moveSpeed = MoveVector * b_move*200.0f;
		CVector3 nextmove = m_position + m_moveSpeed;
		if ((nextmove.x > E_PosR.x) || (nextmove.x < E_PosL.x)) {
			b_move *= -1.0f;
		}

		//回転。
		qBias.SetRotationDeg(CVector3::AxisY(), rot);
		m_rotation.Multiply(qBias);

		rot_sum += rot;//合計角度。

		if ((rot_sum <= -60.0f) || (rot_sum >= 60.0f))
		{
			rot *= -1.0f;
			count++;
		}
	}
	else if((rot_sum <= -2.0f) || (rot_sum >= 2.0f)){
		//横移動。
		m_moveSpeed = MoveVector * b_move*400.0f;
		CVector3 nextmove = m_position + m_moveSpeed;
		if ((nextmove.x > E_PosR.x) || (nextmove.x < E_PosL.x)) {
			b_move *= -1.0f;
		}
		//回転。
		qBias.SetRotationDeg(CVector3::AxisY(), rot);
		m_rotation.Multiply(qBias);
		rot_sum += rot;//合計角度。
	}
	else {
		count = 0;
		bulletcount = 0;
		rot_sum = 0;
		rot = 3.0f;
		b_move = -1.0f;
		attack = move;
	}
}