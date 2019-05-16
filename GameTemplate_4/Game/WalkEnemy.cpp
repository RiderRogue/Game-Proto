#include "stdafx.h"
#include "WalkEnemy.h"

WalkEnemy::WalkEnemy()
{
}


WalkEnemy::~WalkEnemy()
{
}


bool WalkEnemy::Start()
{
	//�G��HP�̍ő�l��ݒ�B
	const int enemy_HP_MAX = 10;

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
	m_position_center = { m_position.x,m_position.y + (enemy_height / 2),m_position.z };
	return true;
}
void WalkEnemy::Update()
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

	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//�ړ��B

	//�_���[�W�����B
	m_position_center = { m_position.x,m_position.y + (enemy_height / 2),m_position.z };

	EnemyShot();
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
}