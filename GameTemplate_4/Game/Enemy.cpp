#include "stdafx.h"
#include "Enemy.h"


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

	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"Assets/modelData/unityChan.cmo");
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
	player = FindGO<Player>("Player");
	enemy_HP = enemy_HP_MAX;
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

	//m_moveSpeed += {1.0f,0.0f,0.0f};
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//�ړ��B

	CVector3 m_position_center = { m_position.x,m_position.y + (enemy_height / 2),m_position.z };
	bool bullethitflag = G_Player_BulletManager().EnemyHit(m_position_center);
	if (bullethitflag == true) {
		//��e���Ă����
		enemyDamage(10);//10�����B
	}
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
}
void Enemy::Draw()
{
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
void Enemy::enemyDamage(int damage)
{
	//�󂯂��_���[�W���AHP��������B
	enemy_HP -= damage;

	if (enemy_HP <= 0) {
		enemy_deathflag = true;
		//G_EnemyManager()
	}
}
void Enemy::SetShadowCasters()
{
	//�V���h�E�L���X�^�[��o�^�B
	Shadow_map().RegistShadowCaster(&m_model);
}