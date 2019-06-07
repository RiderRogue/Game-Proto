#include "stdafx.h"
#include "Enemy.h"
#include "Physics/CollisionAttr.h"

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

	m_moveSpeed = m_rite * (-100.0f);
	m_moveSpeed.y -= 400.0f;//�d�́B
	m_moveSpeed += m_premoveSpeed;
	m_premoveSpeed = CVector3::Zero();
	//m_moveSpeed += {1.0f,0.0f,0.0f};
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
		CVector3 vDist;
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
void Enemy::enemyDamage(int damage)
{
	//�󂯂��_���[�W���AHP��������B
	enemy_HP -= damage;
	damagecount++;

	if (enemy_HP <= 0) {
		enemy_deathflag = true;
	}
}

void Enemy::enemyDamage_Blackhole(int damage, CVector3 b_position)
{
	//�u���b�N�z�[���̋z���͈͂��𒲂ׂ�B
	CVector3 vDist;
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