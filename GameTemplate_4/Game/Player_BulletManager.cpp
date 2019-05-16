#include "stdafx.h"
#include "Player_BulletManager.h"




Player_BulletManager::Player_BulletManager()
{
}


Player_BulletManager::~Player_BulletManager()
{

}

void Player_BulletManager::OnDestroy()
{
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//���˂��Ă���e����
		if (bullet[i].flag == true) {
			//���̂��폜�B
			bullet[i].m_bulletCon.RemoveRigidBoby();
		}
	}
}



void Player_BulletManager::Start()
{
	//cmo�t�@�C���̓ǂݍ��݁B
	//m_modelproto.Init(L"Assets/modelData/bullet.cmo");
	//m_modelproto.SetDirectionLight(5.0f,5.0f,0.0f);
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//flag��false�ɂ��āA���˂���Ă��Ȃ��悤�ɏ���������B
		bullet[i].flag = false;
		bullet[i].desflag = false;
		bullet[i].m_model.Init(L"Assets/modelData/bullet.cmo");
		//bullet[i].m_model.Init(L"Assets/modelData/EnemyBullet.cmo");
		bullet[i].m_model.SetDirectionLight(5.0f, 5.0f, 0.0f);
	}

	for (int i = 0; i < Player_Missile_NUM; i++) {
		//flag��false�ɂ��āA���˂���Ă��Ȃ��悤�ɏ���������B
		missile[i].flag = false;
		missile[i].desflag = false;
		missile[i].m_model.Init(L"Assets/modelData/bullet.cmo");
		missile[i].m_model.SetDirectionLight(5.0f, 5.0f, 0.0f);
		//���b�N�I���ԍ��̏������B
		missile[i].lockon_enemyNumber = -1;
	}
	//�~�T�C���A���B�����55���B
	missile_ammo_NUM = 55;
}

void Player_BulletManager::Update()
{
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//���˂��Ă���e����
		if (bullet[i].flag == true) {
			//�ʏ�e�B
			Normalbullet_move(i);
		}
	}

	for (int i = 0; i < Player_Missile_NUM; i++) {
		//���˂��Ă���e����
		if (missile[i].flag == true) {
			//�ʏ�e�B
			missile_move(i);
		}
	}
	//�e���X�e�[�W�ƐڐG�����Ƃ��Ɏ��S�t���O��������B
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//���˂��Ă���e����
		if (bullet[i].flag == true) {
			if (bullet[i].m_bulletCon.Gethit() == true) {
				//�X�e�[�W�ɏՓ˂����ۂɁA���S�t���O��������B
				bullet[i].desflag = true;
			}
		}
	}

	//�~�T�C�����X�e�[�W�ƐڐG�����Ƃ��Ɏ��S�t���O��������B
	for (int i = 0; i < Player_Missile_NUM; i++) {
		//���˂��Ă���e����
		if (missile[i].flag == true) {
			if (missile[i].m_bulletCon.Gethit() == true) {
				//�X�e�[�W�ɏՓ˂����ۂɁA���S�t���O��������B
				missile[i].desflag = true;
			}
		}
	}
	//�e�����˂���Ĉ�苗���i�񂾂Ƃ��Ɏ��S�t���O��������B
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		if (bullet[i].flag == true) {
			if (bullet[i].lengthcount >= 20) {
				//��苗���i�񂾍ۂɁA���S�t���O��������B
				bullet[i].desflag = true;
			}
		}
	}

	//�~�T�C�������˂���Ĉ�苗���i�񂾂Ƃ��Ɏ��S�t���O��������B
	for (int i = 0; i < Player_Missile_NUM; i++) {
		if (missile[i].flag == true) {
			if (missile[i].lengthcount >= 300) {
				//��苗���i�񂾍ۂɁA���S�t���O��������B
				missile[i].desflag = true;
			}
		}
	}
	//���S�t���O���������e���폜����B
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		if (bullet[i].desflag == true) {
			//���̂��폜�B
			bullet[i].m_bulletCon.RemoveRigidBoby();
			//�폜���ꂽ�̂Ńt���O�����Z�b�g�B
			bullet[i].flag = false;
			bullet[i].desflag = false;
		}
	}

	//���S�t���O���������~�T�C�����폜����B
	for (int i = 0; i < Player_Missile_NUM; i++) {
		if (missile[i].desflag == true) {
			//���̂��폜�B
			missile[i].m_bulletCon.RemoveRigidBoby();
			//�폜���ꂽ�̂Ńt���O�����Z�b�g�B
			missile[i].flag = false;
			missile[i].desflag = false;
		}
	}

	if (count >= 1) {
		count++;
	}
	if (count >= 5) {
		count = 0;
	}
}

void Player_BulletManager::bulletShot(const CVector3 p_position, const CVector3 p_forward)
{
	if (count == 0) {
		for (int i = 0; i < Player_Bullet_NUM; i++) {
			if (bullet[i].flag == false) {
				bullet[i].flag = true;
				bullet[i].desflag = false;
				bullet[i].m_position = p_position;
				bullet[i].m_forward = p_forward;
				bullet[i].m_position += p_forward * 90.0f;
				bullet[i].m_position.y += 90.0f;
				bullet[i].m_bulletCon.Init(
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

void Player_BulletManager::missileShot(const CVector3 p_position, const CVector3 p_forward)
{
	//�e�؂�Ȃ甭�˂��Ȃ��B
	if (missile_ammo_NUM <= 0) {
		for (int i = 0; i < Player_Missile_NUM; i++) {
			if (missile[i].flag == false) {
				missile[i].flag = true;
				missile[i].desflag = false;
				missile[i].m_position = p_position;
				missile[i].m_forward = p_forward;
				missile[i].m_position += p_forward * 90.0f;
				missile[i].m_position.y += 90.0f;
				missile[i].m_bulletCon.Init(
					10.0f,
					missile[i].m_position
				);
				missile[i].lengthcount = 0;
				missile[i].m_model.UpdateWorldMatrix(missile[i].m_position, CQuaternion::Identity(), CVector3::One());
				break;
			}
		}
	}
}
void Player_BulletManager::EnemyHit(Enemy* enemy)
{
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//���˂��Ă���e����
		if (bullet[i].flag == true) {
			CVector3 v = bullet[i].m_position - enemy->GetPosition_center();
			float len = v.Length();//����
								   //�G�ƏՓ˂���(������30.0f�ȉ��Ȃ�)��
			if (len <= 90.0f) {
				//���������e�̎��S�t���O��������B
				bullet[i].desflag = true;
				//�G�ƒe�̋����𑪂�A�������Ă���΃_���[�W�B
				enemy->enemyDamage(BulletDamage);
			}
		}
	}
}

void Player_BulletManager::Normalbullet_move(int bulletNumber)
{
	CVector3 m_moveSpeed = CVector3::Zero();
	m_moveSpeed += bullet[bulletNumber].m_forward*bulletmoveSpeed;
	bullet[bulletNumber].m_position = bullet[bulletNumber].m_bulletCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//�ړ��B
																															//���[���h�s��̍X�V�B
	bullet[bulletNumber].m_model.UpdateWorldMatrix(bullet[bulletNumber].m_position, CQuaternion::Identity(), CVector3::One());
	bullet[bulletNumber].lengthcount += 1;
}

void Player_BulletManager::missile_move(int missileNumber)
{

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