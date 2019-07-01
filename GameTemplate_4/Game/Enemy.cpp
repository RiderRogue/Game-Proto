#include "stdafx.h"
#include "Enemy.h"
#include "Physics/CollisionAttr.h"
#include "EnemyBulletManager.h"

Enemy::Enemy()
{
}


Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	//敵のHPの最大値を設定。
	const int enemy_HP_MAX = 100;
	BulletRange = 4000.0f;
	e_state = Enemy_Search;
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/unityChan.cmo");
	t_model.Init(L"Assets/modelData/TargetSight01.cmo");
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
	//現在はカプセル
	m_charaCon.Init(
		enemy_weight,
		enemy_height,
		m_position
	);
	//敵弾の半径。
	float radius = 10.0f;
	m_charaCon.Init_bullet(
		radius,
		m_position
	);
	m_charaCon.GetRigidBody()->GetBody()->setUserIndex(enCollisionAttr_Enemy);
	player = FindGO<Player>("Player");
	enemy_HP = enemy_HP_MAX;
	m_position_center = m_charaCon.Getbullet_position();
	Search_movespeed = 1000.0f;
	return true;
}

void Enemy::Update()
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
	switch (e_state)
	{
	case Enemy_Search:		
		SearchUpdate();
		break;
	case Enemy_Attack:
		AttackUpdate();
		break;
	default:
		SearchPlayer();
		break;
	}
	
	m_moveSpeed += m_premoveSpeed;
	m_premoveSpeed = CVector3::Zero();
	
	//m_moveSpeed = m_charaCon.ReflectWall(m_moveSpeed);
	
	m_position = m_charaCon.EnemyExecute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//移動。
	//ダメージ処理。
	m_position_center = m_charaCon.Getbullet_position();

	EnemyShot();

	if (damagecount >= 2) {
		m_model.ReturnDirectionDamage();
		damagecount = 0;
	}
	if (damagecount==1) {
		m_model.SetDirectionDamage();
		damagecount++;
	}
	
	if (count >= 1) {
		count++;
	}
	if (count >= 5) {
		count = 0;
	}

	qBias.SetRotationDeg(CVector3::AxisY(),15.0f);
	t_rotation.Multiply(qBias);
	qBias.SetRotationDeg(CVector3::AxisZ(), 15.0f);
	t_rotation.Multiply(qBias);
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
	t_model.UpdateWorldMatrix(m_position_center, t_rotation, CVector3::One());
}
void Enemy::SearchUpdate()
{
	static int count = 0;
	static float anglenum = 0;
	m_moveSpeed = m_forward * Search_movespeed;
	count++;
	//一定距離移動したら別行動
	if (count >= 100) {
		qBias.SetRotationDeg(CVector3::AxisY(), 10.0f);
		m_rotation.Multiply(qBias);
		anglenum += 10.0f;
		if (anglenum >= 180.0f) {
			count = 0;
			anglenum = 0;
		}
	}
	//SearchPlayer();
}
void Enemy::SearchPlayer()
{
	//プレイヤーとの距離を求める。
	vDist.Subtract(player->GetPosition_center(), m_position_center);
	float distTmp = vDist.Length();
	//射程範囲内の場合
	if (distTmp <= BulletRange) {
		float angle = VectorAngleDeg(vDist);
		//プレイヤーとの角度が60度以内なら
		if (angle <= 60.0f)
		{
			bool shotFlag = m_charaCon.EnemyofPlayerVector(player->GetPosition_center());
			//敵からプレイヤーへの射線が通れば、
			if (shotFlag == false) {
				//見つけた。
				e_state = Enemy_Attack;
			}
		}
	}
}

void Enemy::AttackUpdate()
{

}
void Enemy::Draw()
{
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
	
}
void Enemy::EnemyShot()
{
	//敵弾の発射間隔を作る。
	if (count == 0) {
		//プレイヤーとの距離を求める。
		vDist.Subtract(player->GetPosition_center(), m_position_center);
		float distTmp = vDist.Length();
		//射程範囲内の場合
		if (distTmp <= BulletRange) {
			float angle = VectorAngleDeg(vDist);
			//プレイヤーとの角度が60度以内なら
			if (angle <= 60.0f)
			{
				bool shotFlag = m_charaCon.EnemyofPlayerVector(player->GetPosition_center());
				//敵からプレイヤーへの射線が通れば、
				if (shotFlag == false) {
					//射撃。
					vDist.Normalize();
					G_EnemyBulletManager().bulletShot(m_position_center, vDist);
					count = 1;
				}
			}
		}
	}



	
}

void Enemy::PlayertoRotation()
{
	static float angle;
	vDist.Subtract(player->GetPosition_center(), m_position);
	vDist.Normalize();
	angle=vDist.Dot(m_forward);
	//ラップ。
	if (angle >= 1.0f) {
		angle = 1.0f;
	}
	else if (angle <= -1.0f) {
		angle = -1.0f;
	}
	angle=CMath::RadToDeg(acos(angle));
	if (angle >= 5.0f) {
		angle = 5.0f;
	}
	CVector3 rotAxis;
	rotAxis.Cross(m_forward, vDist);
	rotAxis.Normalize();
	//回転。
	qBias.SetRotationDeg(rotAxis, angle);
	m_rotation.Multiply(qBias);
}
void Enemy::enemyDamage(int damage)
{
	//受けたダメージ分、HPから引く。
	enemy_HP -= damage;
	damagecount++;
	e_state = Enemy_Attack;
	if (enemy_HP <= 0) {
		enemy_deathflag = true;
	}
}

void Enemy::enemyDamage_Blackhole(int damage, CVector3 b_position)
{
	//ブラックホールの吸収範囲かを調べる。
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
	e_state = Enemy_Attack;
}

float Enemy::VectorAngleDeg(CVector3 c)
{
	c.y = 0.0f;
	c.Normalize();//向きVectorにする。
	float kaku = acosf(c.Dot(m_forward));//２つのべクトルの内積のアークコサインを求める。(ラジアン)

	float degree = CMath::RadToDeg(kaku);

	return degree;
}
void Enemy::SetShadowCasters()
{
	//シャドウキャスターを登録。
	Shadow_map().RegistShadowCaster(&m_model);
}

void Enemy::PostDraw()
{
	t_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}