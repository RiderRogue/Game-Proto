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
	//�G��HP�̍ő�l��ݒ�B
	const int enemy_HP_MAX = 100;
	BulletRange = 4000.0f;
	e_state = Enemy_Search;
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"Assets/modelData/unityChan.cmo");
	t_model.Init(L"Assets/modelData/TargetSight01.cmo");
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
	m_forward.Normalize();
	//�E����
	m_rite.x = mRot.m[0][0];
	m_rite.y = mRot.m[0][1];
	m_rite.z = mRot.m[0][2];
	m_rite.Normalize();
	//���݂̓J�v�Z��
	m_charaCon.Init(
		enemy_weight,
		enemy_height,
		m_position
	);
	//�G�e�̔��a�B
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
	//�E����
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
	
	m_position = m_charaCon.EnemyExecute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//�ړ��B
	//�_���[�W�����B
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
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
	t_model.UpdateWorldMatrix(m_position_center, t_rotation, CVector3::One());
}
void Enemy::SearchUpdate()
{
	static int count = 0;
	static float anglenum = 0;
	m_moveSpeed = m_forward * Search_movespeed;
	count++;
	//��苗���ړ�������ʍs��
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
	//�v���C���[�Ƃ̋��������߂�B
	vDist.Subtract(player->GetPosition_center(), m_position_center);
	float distTmp = vDist.Length();
	//�˒��͈͓��̏ꍇ
	if (distTmp <= BulletRange) {
		float angle = VectorAngleDeg(vDist);
		//�v���C���[�Ƃ̊p�x��60�x�ȓ��Ȃ�
		if (angle <= 60.0f)
		{
			bool shotFlag = m_charaCon.EnemyofPlayerVector(player->GetPosition_center());
			//�G����v���C���[�ւ̎ː����ʂ�΁A
			if (shotFlag == false) {
				//�������B
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
	//�G�e�̔��ˊԊu�����B
	if (count == 0) {
		//�v���C���[�Ƃ̋��������߂�B
		vDist.Subtract(player->GetPosition_center(), m_position_center);
		float distTmp = vDist.Length();
		//�˒��͈͓��̏ꍇ
		if (distTmp <= BulletRange) {
			float angle = VectorAngleDeg(vDist);
			//�v���C���[�Ƃ̊p�x��60�x�ȓ��Ȃ�
			if (angle <= 60.0f)
			{
				bool shotFlag = m_charaCon.EnemyofPlayerVector(player->GetPosition_center());
				//�G����v���C���[�ւ̎ː����ʂ�΁A
				if (shotFlag == false) {
					//�ˌ��B
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
	//���b�v�B
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
	//��]�B
	qBias.SetRotationDeg(rotAxis, angle);
	m_rotation.Multiply(qBias);
}
void Enemy::enemyDamage(int damage)
{
	//�󂯂��_���[�W���AHP��������B
	enemy_HP -= damage;
	damagecount++;
	e_state = Enemy_Attack;
	if (enemy_HP <= 0) {
		enemy_deathflag = true;
	}
}

void Enemy::enemyDamage_Blackhole(int damage, CVector3 b_position)
{
	//�u���b�N�z�[���̋z���͈͂��𒲂ׂ�B
	vDist.Subtract(b_position, m_position_center);
	//�u���b�N�z�[���̒��S�ɗ�����
	if (vDist.Length() <= 60.0f) {
		//�󂯂��_���[�W���AHP��������B
		enemy_HP -= damage;
		if (enemy_HP <= 0) {
			enemy_deathflag = true;
		}
	}

	vDist.Normalize();//���K���B
	m_premoveSpeed = vDist * (800.0f);
	e_state = Enemy_Attack;
}

float Enemy::VectorAngleDeg(CVector3 c)
{
	c.y = 0.0f;
	c.Normalize();//����Vector�ɂ���B
	float kaku = acosf(c.Dot(m_forward));//�Q�̂׃N�g���̓��ς̃A�[�N�R�T�C�������߂�B(���W�A��)

	float degree = CMath::RadToDeg(kaku);

	return degree;
}
void Enemy::SetShadowCasters()
{
	//�V���h�E�L���X�^�[��o�^�B
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