#include "stdafx.h"
#include "Player_BulletManager.h"
#include "graphics/EffekseerManager.h"




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

	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		//���˂��Ă���e����
		if (blackhole[i].flag == true) {
			//���̂��폜�B
			blackhole[i].m_bulletCon.RemoveRigidBoby();
		}
	}

	//�G�t�F�N�g��j���B
	if (m_sampleEffect != nullptr) {
		m_sampleEffect->Release();
	}

	//�G�t�F�N�g��j���B
	if (m_blackholeEffect != nullptr) {
		m_blackholeEffect->Release();
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

	for (int i = 0; i < Player_Mine_NUM; i++) {
		mine[i].flag = false;
		mine[i].desflag = false;
		mine[i].explosion = false;
		mine[i].m_model.Init(L"Assets/modelData/Mine.cmo");
		mine[i].blastrange = 600.0f;//�����͈̔͂�ݒ�B
	}

	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		//flag��false�ɂ��āA���˂���Ă��Ȃ��悤�ɏ���������B
		blackhole[i].flag = false;
		blackhole[i].desflag = false;
		blackhole[i].time = 0.0f;
		blackhole[i].m_model.Init(L"Assets/modelData/hole.cmo");
		blackhole[i].m_model.SetDirectionLight(5.0f, 5.0f, 0.0f);
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

	//�T���v���̃G�t�F�N�g�����[�h����B
	m_sampleEffect = Effekseer::Effect::Create(
		G_EffekseerManager().GetManager(),
		(const EFK_CHAR*)L"Assets/effect/black hole.efk",
		20.0f
	);

	//�u���b�N�z�[���̃G�t�F�N�g�����[�h����B
	m_blackholeEffect = Effekseer::Effect::Create(
		G_EffekseerManager().GetManager(),
		(const EFK_CHAR*)L"Assets/effect/black hole.efk",
		20.0f
	);
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

	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		//���˂��Ă���u���b�N�z�[���e����
		if (blackhole[i].flag == true) {
			//�u���b�N�z�[���e�B
			Blackhole_move(i);
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

	//�u���b�N�z�[���e���X�e�[�W�ƐڐG�����Ƃ��Ɏ��S�t���O��������B
	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		//���˂��Ă���e����
		if (blackhole[i].flag == true) {
			if (blackhole[i].m_bulletCon.Gethit() == true) {
				//�X�e�[�W�ɏՓ˂����ۂɁA�����t���O��������B
				if (blackhole[i].explosion!=true) {
					G_EffekseerManager().Play(m_blackholeEffect, blackhole[i].m_position);
					blackhole[i].explosion = true;
				}			
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

	//�u���b�N�z�[���e�����˂���Ĉ�苗���i�񂾂Ƃ��Ɏ��S�t���O��������B
	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		if (blackhole[i].flag == true) {
			if (blackhole[i].lengthcount >= 30) {
				//�X�e�[�W�ɏՓ˂����ۂɁA�����t���O��������B
				if (blackhole[i].explosion != true) {
					G_EffekseerManager().Play(m_blackholeEffect, blackhole[i].m_position);
					blackhole[i].explosion = true;
				}
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

	//���S�t���O���������}�C�����폜����B
	for (int i = 0; i < Player_Mine_NUM; i++) {
		if (mine[i].desflag == true) {
			
			//�Đ��B
			m_mineEffectHandle = G_EffekseerManager().Play(m_sampleEffect, mine[i].m_position);
			//�폜���ꂽ�̂Ńt���O�����Z�b�g�B
			mine[i].flag = false;
			mine[i].desflag = false;
		}
	}

	//���S�t���O���������u���b�N�z�[���e���폜����B
	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		if (blackhole[i].desflag == true) {
			//���̂��폜�B
			blackhole[i].m_bulletCon.RemoveRigidBoby();
			//�폜���ꂽ�̂Ńt���O�����Z�b�g�B
			blackhole[i].flag = false;
			blackhole[i].desflag = false;
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
	
	if (minecount >= 1) {
		minecount++;
	}
	if (minecount >= 10) {
		minecount = 0;
	}

	if (blackholecount >= 1) {
		blackholecount++;
	}
	if (blackholecount >= 50) {
		blackholecount = 0;
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

void Player_BulletManager::mineShot(const CVector3 p_position)
{
	if (count == 0) {
		for (int i = 0; i < Player_Mine_NUM; i++) {
			if (mine[i].flag == false) {
				mine[i].flag = true;
				mine[i].desflag = false;
				mine[i].explosion = false;//�����t���O���������B
				mine[i].m_position = p_position;
				mine[i].m_position.y += 50.0f;
				minecount = 1;
				mine[i].m_model.UpdateWorldMatrix(mine[i].m_position, CQuaternion::Identity(), CVector3::One());
				break;
			}
		}
	}
}

void Player_BulletManager::BlackholeShot(const CVector3 p_position, const CVector3 p_forward)
{
	if (blackholecount == 0) {
		for (int i = 0; i < Player_Blackhole_NUM; i++) {
			if (blackhole[i].flag == false) {
				blackhole[i].flag = true;
				blackhole[i].desflag = false;
				blackhole[i].m_position = p_position;
				blackhole[i].m_forward = p_forward;
				blackhole[i].m_position += p_forward * 90.0f;
				blackhole[i].m_position.y += 90.0f;
				blackhole[i].explosion = false;//�����t���O���������B
				blackhole[i].time = 0.0f;
				blackhole[i].m_bulletCon.Init(
					10.0f,
					blackhole[i].m_position
				);
				blackhole[i].lengthcount = 0;
				blackholecount = 1;
				blackhole[i].m_model.UpdateWorldMatrix(blackhole[i].m_position, CQuaternion::Identity(), CVector3::One());
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
								   //�G�ƏՓ˂���(������90.0f�ȉ��Ȃ�)��
			if (len <= 90.0f) {
				//���������e�̎��S�t���O��������B
				bullet[i].desflag = true;
				//�G�ƒe�̋����𑪂�A�������Ă���΃_���[�W�B
				enemy->enemyDamage(BulletDamage);
			}
		}
	}
}

void Player_BulletManager::mineexplosion() {
	for (int i = 0; i < Player_Mine_NUM; i++) {
		//�ݒu���Ă���}�C������
		if (mine[i].flag == true) {
			//���������e�̎��S�t���O��������B
			mine[i].explosion = true;
			mine[i].desflag = true;
		}
	}
}

void Player_BulletManager::EnemyHitMine(Enemy* enemy)
{
	for (int i = 0; i < Player_Mine_NUM; i++) {
		//�ݒu���Ă���}�C������
		if (mine[i].flag == true) {
			CVector3 v = mine[i].m_position - enemy->GetPosition_center();
			float len = v.Length();//����
								   //�G�ƏՓ˂���(������200.0f�ȉ��Ȃ�)��
			if ((mine[i].explosion == true) && (len <= mine[i].blastrange)) {
				//�G�ƒe�̋����𑪂�A�������Ă���΃_���[�W�B
				enemy->enemyDamage(MineDamage);
			}
			else if (len <= 200.0f) {
				//���������e�̎��S�t���O��������B
				mine[i].explosion = true;
				mine[i].desflag = true;
				//�G�ƒe�̋����𑪂�A�������Ă���΃_���[�W�B
				enemy->enemyDamage(MineDamage);
			}
		}
	}
}

void Player_BulletManager::EnemyHitBlackhole(Enemy* enemy)
{
	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		//���˂��Ă���u���b�N�z�[������
		if (blackhole[i].flag == true) {
			CVector3 v = blackhole[i].m_position - enemy->GetPosition_center();
			float len = v.Length();//����
								   //�G�ƏՓ˂���(�������z���͈͈ȉ��Ȃ�)��
			if ((blackhole[i].explosion == true) && (len <= blackholeAbsorb)) {
				//�G�ƒe�̋����𑪂�A�������Ă���΃_���[�W�B
				enemy->enemyDamage_Blackhole(BlackholeDamage, blackhole[i].m_position);
				
			}
			else if ((blackhole[i].explosion==false)&&(len <= 90.0f)) {
				//���������e�̔����t���O�������āA�G�t�F�N�g���Đ�����B
				blackhole[i].explosion = true;
				G_EffekseerManager().Play(m_blackholeEffect, blackhole[i].m_position);
				//�G�ƒe�̋����𑪂�A�������Ă���΃_���[�W�B
				enemy->enemyDamage_Blackhole(BlackholeDamage, blackhole[i].m_position);
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

void Player_BulletManager::Blackhole_move(int bulletNumber)
{
	//�������Ă��Ȃ�������i�ށB
	if (blackhole[bulletNumber].explosion != true) {
		CVector3 m_moveSpeed = CVector3::Zero();
		m_moveSpeed += blackhole[bulletNumber].m_forward*bulletmoveSpeed / 20;
		blackhole[bulletNumber].m_position = blackhole[bulletNumber].m_bulletCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//�ړ��B
																																//���[���h�s��̍X�V�B
		blackhole[bulletNumber].m_model.UpdateWorldMatrix(blackhole[bulletNumber].m_position, CQuaternion::Identity(), CVector3::One());
		blackhole[bulletNumber].lengthcount += 1;
	}
	else {
		float holesc = 1.0f+blackhole[bulletNumber].time/5.0f;
		if (holesc >= 6.5f) {
			holesc = 6.5f;
		}
		blackhole[bulletNumber].m_model.UpdateWorldMatrix(blackhole[bulletNumber].m_position, CQuaternion::Identity(), { holesc,holesc,holesc});
		blackhole[bulletNumber].time++;
		if (blackhole[bulletNumber].time >= blackholeTime) {
			//�������Ԃ��߂����玀�S�t���O��������B
			blackhole[bulletNumber].desflag = true;
		}
	}
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

	for (int i = 0; i < Player_Mine_NUM; i++) {
		if (mine[i].flag == true) {
			mine[i].m_model.Draw(
				enRenderMode_Normal,
				g_camera3D.GetViewMatrix(),
				g_camera3D.GetProjectionMatrix()
			);
		}
	}

	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		if ((blackhole[i].flag == true)&&(blackhole[i].explosion != true)) {
			blackhole[i].m_model.Draw(
				enRenderMode_Normal,
				g_camera3D.GetViewMatrix(),
				g_camera3D.GetProjectionMatrix()
			);
		}
	}
}