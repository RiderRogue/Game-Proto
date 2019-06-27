#include "stdafx.h"
#include "GroundingLauncher.h"
#include "EnemyBulletManager.h"


GroundingLauncher::GroundingLauncher()
{
}


GroundingLauncher::~GroundingLauncher()
{
}

bool GroundingLauncher::Start()
{
	enemy_weight = 180.0f;
	enemy_height = 90.0f;
	//�G��HP�̍ő�l��ݒ�B
	const int enemy_HP_MAX = 100;
	BulletRange = 1500.0f;
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"Assets/modelData/GroundingLauncher.cmo");
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

	GL_AnimationClips[HatchOpen].Load(L"Assets/animData/LauncherHatch_Open.tka");
	GL_AnimationClips[HatchClose].Load(L"Assets/animData/LauncherHatch_Close.tka");

	GL_AnimationClips[HatchOpen].SetLoopFlag(false);
	GL_AnimationClips[HatchClose].SetLoopFlag(false);
	GL_Animation.Init(
		m_model,
		GL_AnimationClips,
		Animnum
	);
	return true;
}

void GroundingLauncher::Update()
{
	//�G�e�̔��ˊԊu�����B
	if (count == 0) {
		//�v���C���[�Ƃ̋��������߂�B
		CVector3 vDist;
		vDist.Subtract(player->GetPosition_center(), m_position_center);
		float distTmp = vDist.Length();
		//�˒��͈͓��̏ꍇ
		if (distTmp <= BulletRange) {
			bool shotFlag = m_charaCon.EnemyofPlayerVector(player->GetPosition_center());
			//�G����v���C���[�ւ̎ː����ʂ�΁A
			if (shotFlag == false) {
				//�ˌ��B
				GL_Animation.Play(HatchOpen, 0.25);
				vDist.Normalize();
				G_EnemyBulletManager().VLSShot(m_position_center, vDist);
				count = 1;
			}
		}
		else
		{
			GL_Animation.Play(HatchClose, 0.25);
		}
	}

	if (count >= 0) {
		count++;
	}
	
	if (count >= 8) {
		count = 0;
	}
	GL_Animation.Update(1.0f / 30.0f);
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One()*3.0f);
}

void GroundingLauncher::PostDraw()
{
	
}