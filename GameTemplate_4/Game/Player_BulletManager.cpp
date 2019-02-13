#include "stdafx.h"
#include "Player_BulletManager.h"




Player_BulletManager::Player_BulletManager()
{
}


Player_BulletManager::~Player_BulletManager()
{
}



bool Player_BulletManager::Start()
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_modelproto.Init(L"Assets/modelData/bullet.cmo");
	m_modelproto.SetDirectionLight(5.0f,5.0f,0.0f);
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//flag��false�ɂ��āA���˂���Ă��Ȃ��悤�ɏ���������B
		bullet[i].flag = false;
		bullet[i].desflag = false;
		bullet[i].m_model = m_modelproto;
	}
	return true;
}

void Player_BulletManager::Update()
{
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//���˂��Ă���e����
		if (bullet[i].flag == true) {
			CVector3 m_moveSpeed = CVector3::Zero();
			m_moveSpeed += bullet[i].m_forward*bulletmoveSpeed*100.0f;
			bullet[i].m_position = bullet[i].m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//�ړ��B
			//���[���h�s��̍X�V�B
			bullet[i].m_model.UpdateWorldMatrix(bullet[i].m_position, CQuaternion::Identity(), CVector3::One());
			bullet[i].lengthcount+=1;
		}
	}

	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//���˂��Ă���e����
		if (bullet[i].flag == true) {
			if (bullet[i].m_charaCon.Gethit() == true) {
				bullet[i].desflag = true;
			}
		}
	}

	for (int i = 0; i < Player_Bullet_NUM; i++) {
		if (bullet[i].flag == true) {
			if (bullet[i].lengthcount >= 20) {
				//��萔�i�񂾍ۂɁA���S�t���O��������B
				bullet[i].desflag = true;
			}
		}
	}
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		if (bullet[i].desflag == true) {
			//���̂��폜�B
			bullet[i].m_charaCon.RemoveRigidBoby();
			//�폜���ꂽ�̂Ńt���O�����Z�b�g�B
			bullet[i].flag = false;
			bullet[i].desflag = false;
		}
	}

	if (count >= 1) {
		count++;
	}
	if (count >= 5) {
		count = 0;
	}
}

void Player_BulletManager::Shot(const CVector3 p_position, const CVector3 p_forward)
{
	if (count==0) {
		for (int i = 0; i < Player_Bullet_NUM; i++) {
			if (bullet[i].flag == false) {
				bullet[i].flag = true;
				bullet[i].desflag = false;
				bullet[i].m_position = p_position;
				bullet[i].m_forward = p_forward;
				bullet[i].m_position += p_forward*90.0f;
				bullet[i].m_position.y += 90.0f;
				bullet[i].m_charaCon.Init(
					10.0f,
					bullet[i].m_position
				);
				bullet[i].lengthcount = 0;
				count = 1;
				bullet[i].m_model.UpdateWorldMatrix(bullet[i].m_position, CQuaternion::Identity(), CVector3::One());
				break;
			}
		}
	}
}

void Player_BulletManager::Draw()
{
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		if (bullet[i].flag == true) {
			bullet[i].m_model.Draw(
				enRenderMode_Normal,
				g_camera3D.GetViewMatrix(),
				g_camera3D.GetProjectionMatrix()
			);
		}
	}
}