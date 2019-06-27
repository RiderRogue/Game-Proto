#include "stdafx.h"
#include "Player_BulletManager.h"
#include "graphics/EffekseerManager.h"


void Player_BulletState::Init(CVector3 p_position, CVector3 p_forward)
{
	desflag = false;
	m_position = p_position;
	m_forward = p_forward;
	m_position += p_forward * 90.0f;
	m_position.y += 90.0f;
	m_bulletCon.Init(
		10.0f,
		m_position
	);
	m_bulletCon.GetRigidBody()->GetBody()->setUserIndex(enCollisionAttr_PlayerBullet);
	lengthcount = 0;
	m_model.Init(L"Assets/modelData/bullet.cmo");
	m_model.SetDirectionLight(5.0f, 5.0f, 0.0f);
	m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	bulletmoveSpeed = 10000.0f;
	damage = 10;
	lengthcount_MAX = 30;
}
void Player_BulletState::bulletmove()
{
	CVector3 m_moveSpeed = CVector3::Zero();
	m_moveSpeed += m_forward * bulletmoveSpeed;
	m_position = m_bulletCon.ReflectExecute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//�ړ��B
	//m_forward = m_moveSpeed;//�ړ��x�N�g���̕ύX�B
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	lengthcount += 1;

	//�e���X�e�[�W�ƐڐG�����Ƃ��Ɏ��S�t���O��������B
	if (m_bulletCon.Gethit() == true) {
		//�X�e�[�W�ɏՓ˂����ۂɁA���S�t���O��������B
		//desflag = true;
	}
	//�e�����˂���Ĉ�苗���i�񂾂Ƃ��Ɏ��S�t���O��������B
	if (lengthcount >= lengthcount_MAX) {
		//��苗���i�񂾍ۂɁA���S�t���O��������B
		desflag = true;
	}
}
void Player_BulletState::Hitbullet(Enemy* enemy)
{
	CVector3 v = m_position - enemy->GetPosition_center();
	float len = v.Length();//����
						   //�G�ƏՓ˂���(������90.0f�ȉ��Ȃ�)��
	if (len <= 90.0f) {
		//���������e�̎��S�t���O��������B
		desflag = true;
		//�G�ƒe�̋����𑪂�A�������Ă���΃_���[�W�B
		enemy->enemyDamage(damage);
	}
}
void Player_BulletState::Draw() {
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}


void Player_MineState::Init(CVector3 p_position)
{
	desflag = false;
	explosion = false;//�����t���O���������B
	m_position = p_position;
	m_position.y += 50.0f;
	m_model.Init(L"Assets/modelData/Mine.cmo");
	m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	MineDamage = 100;
	blastrange = 600;
}

void Player_MineState::Hitmine(Enemy* enemy)
{
	CVector3 v = m_position - enemy->GetPosition_center();
	float len = v.Length();//����
	 //�G�ƏՓ˂���(������200.0f�ȉ��Ȃ�)��
	if ((explosion == true) && (len <= blastrange)) {
		//�G�ƒe�̋����𑪂�A�������Ă���΃_���[�W�B
		enemy->enemyDamage(MineDamage);
	}
	else if (len <= (blastrange/3)) {
		//���������e�̎��S�t���O��������B
		explosion = true;
		desflag = true;
		//�G�ƒe�̋����𑪂�A�������Ă���΃_���[�W�B
		enemy->enemyDamage(MineDamage);
	}
}
void Player_MineState::Draw()
{
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Player_Blackhole::Init(CVector3 p_position, CVector3 p_forward)
{
	desflag = false;
	m_position = p_position;
	m_forward = p_forward;
	m_position += p_forward * 90.0f;
	m_position.y += 90.0f;
	explosion = false;//�����t���O���������B
	time = 0.0f;
	m_bulletCon.Init(
		10.0f,
		m_position
	);
	lengthcount = 0;
	
	m_model.Init(L"Assets/modelData/hole.cmo");
	m_model.SetDirectionLight(5.0f, 5.0f, 0.0f);
	m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	bulletmoveSpeed = 5000.0f;
	BlackholeDamage = 5;
	blackholeTime = 300.0f;
	lengthcount_MAX = 30;
	blackholeAbsorb = 2000.0f;
}

void Player_Blackhole::bulletmove(Effekseer::Effect* m_blackholeEffect)
{
	//�������Ă��Ȃ�������i�ށB
	if (explosion != true) {
		CVector3 m_moveSpeed = CVector3::Zero();
		m_moveSpeed += m_forward*bulletmoveSpeed / 20;
		m_position = m_bulletCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//�ړ��B
																																	  //���[���h�s��̍X�V�B
		m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
		lengthcount += 1;
	}
	else {
		//���Ԍo�߂Ŋg�傷��B
		float holesc = 1.0f + time / 5.0f;
		if (holesc >= 6.5f) {
			holesc = 6.5f;
		}
		m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), { holesc,holesc,holesc });
		time++;
		if (time >= blackholeTime) {
			//�������Ԃ��߂����玀�S�t���O��������B
			desflag = true;
		}
	}

	if (m_bulletCon.Gethit() == true) {
		//�X�e�[�W�ɏՓ˂����ۂɁA�����t���O��������B
		if (explosion != true) {
			G_EffekseerManager().Play(m_blackholeEffect, m_position);
			explosion = true;
		}
	}

	if (lengthcount >= lengthcount_MAX) {
		//�X�e�[�W�ɏՓ˂����ۂɁA�����t���O��������B
		if (explosion != true) {
			G_EffekseerManager().Play(m_blackholeEffect, m_position);
			explosion = true;
		}
	}
}

void Player_Blackhole::HitBlackhole(Enemy* enemy, Effekseer::Effect* m_blackholeEffect)
{
	CVector3 v = m_position - enemy->GetPosition_center();
	float len = v.Length();//����
	 //�G�ƏՓ˂���(�������z���͈͈ȉ��Ȃ�)��
	if ((explosion == true) && (len <= blackholeAbsorb)) {
		//�G�ƒe�̋����𑪂�A�������Ă���΃_���[�W�B
		enemy->enemyDamage_Blackhole(BlackholeDamage, m_position);

	}
	else if ((explosion == false) && (len <= 90.0f)) {
		//���������e�̔����t���O�������āA�G�t�F�N�g���Đ�����B
		explosion = true;
		G_EffekseerManager().Play(m_blackholeEffect, m_position);
		//�G�ƒe�̋����𑪂�A�������Ă���΃_���[�W�B
		enemy->enemyDamage_Blackhole(BlackholeDamage, m_position);
	}
}
void Player_Blackhole::Draw()
{
	if (explosion != true) {//�u���b�N�z�[������������Ă��Ȃ�������`��B
		m_model.Draw(
			enRenderMode_Normal,
			g_camera3D.GetViewMatrix(),
			g_camera3D.GetProjectionMatrix()
		);
	}
}

CVector3 Player_Blackhole::AbsorbEnemyBullet(CVector3 e_bullet)
{
	CVector3 Absorb = m_position - e_bullet;
	float len = Absorb.Length();//����
	//�u���b�N�z�[������������A�z���͈͂Ȃ�
	if ((explosion == true) && (len <= blackholeAbsorb)) {
		//�z������B
		Absorb.Normalize();
		Absorb *= 2000.0f;
	}
	else {
		//�z������Ȃ��B
		Absorb = CVector3::Zero();
	}
	return Absorb;
}


void Player_BulletManager::OnDestroy()
{
	erasebullet_All();

	//�G�t�F�N�g��j���B
	if (m_blackholeEffect != nullptr) {
		m_blackholeEffect->Release();
	}
}



void Player_BulletManager::Start()
{

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

	//�u���b�N�z�[���̃G�t�F�N�g�����[�h����B
	m_blackholeEffect = Effekseer::Effect::Create(
		G_EffekseerManager().GetManager(),
		(const EFK_CHAR*)L"Assets/effect/black hole.efk",
		20.0f
	);
}

void Player_BulletManager::Update()
{
	for (std::list<Player_BulletState*>::iterator itr = P_BulletList.begin(); itr != P_BulletList.end(); itr++) {
		(*itr)->bulletmove();//�ǔ���̍��W�B
	}

	for (Player_Blackhole* p_black : P_BlackholeList) {
		p_black->bulletmove(m_blackholeEffect);
	}

	for (int i = 0; i < Player_Missile_NUM; i++) {
		//���˂��Ă���e����
		if (missile[i].flag == true) {
			//�ʏ�e�B
			missile_move(i);
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

	
	//�~�T�C�������˂���Ĉ�苗���i�񂾂Ƃ��Ɏ��S�t���O��������B
	for (int i = 0; i < Player_Missile_NUM; i++) {
		if (missile[i].flag == true) {
			if (missile[i].lengthcount >= 300) {
				//��苗���i�񂾍ۂɁA���S�t���O��������B
				missile[i].desflag = true;
			}
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
	if (blackholecount >= 300) {
		blackholecount = 0;
	}
	erasebullet();//�s�v�Ȓe�̍폜�B
}

void Player_BulletManager::bulletShot(const CVector3 p_position, const CVector3 p_forward)
{
	if (count == 0) {//�A�˖h�~
		Player_BulletState* bullet;
		bullet = new(Player_BulletState);
		bullet->Init(p_position, p_forward);
		P_BulletList.push_back(bullet);
		count = 1;
	}
}

void Player_BulletManager::mineShot(const CVector3 p_position)
{
	if (minecount == 0) {//�A�˖h�~
		Player_MineState* mine;
		mine = new(Player_MineState);
		mine->Init(p_position);
		P_MineList.push_back(mine);
		minecount = 1;
	}
}

void Player_BulletManager::BlackholeShot(const CVector3 p_position, const CVector3 p_forward)
{
	if (blackholecount == 0) {//�A�˖h�~
		Player_Blackhole* blackhole;
		blackhole = new(Player_Blackhole);
		blackhole->Init(p_position, p_forward);
		P_BlackholeList.push_back(blackhole);
		blackholecount = 1;
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
	for (Player_BulletState* p_bullet : P_BulletList) {
		p_bullet->Hitbullet(enemy);
	}
}

void Player_BulletManager::mineexplosion() {
	//�S�N���B
	for (Player_MineState* p_mine : P_MineList) {
		p_mine->Setexplosion(true);
		p_mine->Setdesflag(true);
	}
}

void Player_BulletManager::EnemyHitMine(Enemy* enemy)
{
	for (Player_MineState* p_mine : P_MineList) {
		p_mine->Hitmine(enemy);
	}
}

void Player_BulletManager::EnemyHitBlackhole(Enemy* enemy)
{
	for (Player_Blackhole* p_blackhole : P_BlackholeList) {
		p_blackhole->HitBlackhole(enemy, m_blackholeEffect);
	}
}

CVector3 Player_BulletManager::Blackhole_EnemyBullet(CVector3 e_bullet)
{
	
	CVector3 Absorb = CVector3::Zero();
	for (Player_Blackhole* p_blackhole : P_BlackholeList) {
		Absorb =p_blackhole->AbsorbEnemyBullet(e_bullet);
	}
	return Absorb;
}

void Player_BulletManager::missile_move(int missileNumber)
{

}

void Player_BulletManager::erasebullet()
{
	for (std::list<Player_BulletState*>::iterator itr = P_BulletList.begin(); itr != P_BulletList.end(); ) {
		if ((*itr)->Getdesflag() == true) {
			//���S���Ă���΍폜����B
			delete(*itr);
			itr = P_BulletList.erase(itr);
		}
		else {
			itr++;
		}
	}

	for (std::list<Player_MineState*>::iterator itr = P_MineList.begin(); itr != P_MineList.end(); ) {
		if ((*itr)->Getdesflag() == true) {
			//���S���Ă���΍폜����B
			delete(*itr);
			itr = P_MineList.erase(itr);
		}
		else {
			itr++;
		}
	}

	for (std::list<Player_Blackhole*>::iterator itr = P_BlackholeList.begin(); itr != P_BlackholeList.end(); ) {
		if ((*itr)->Getdesflag() == true) {
			//���S���Ă���΍폜����B
			delete(*itr);
			itr = P_BlackholeList.erase(itr);
		}
		else {
			itr++;
		}
	}
}
void Player_BulletManager::erasebullet_All()
{
	for (std::list<Player_BulletState*>::iterator itr = P_BulletList.begin(); itr != P_BulletList.end(); ) {
		delete(*itr);
		itr = P_BulletList.erase(itr);
	}

	for (std::list<Player_MineState*>::iterator itr = P_MineList.begin(); itr != P_MineList.end(); ) {
		delete(*itr);
		itr = P_MineList.erase(itr);
	}

	for (std::list<Player_Blackhole*>::iterator itr = P_BlackholeList.begin(); itr != P_BlackholeList.end(); ) {
		delete(*itr);
		itr = P_BlackholeList.erase(itr);
	}
}
void Player_BulletManager::Draw()
{
	for (std::list<Player_BulletState*>::iterator itr = P_BulletList.begin(); itr != P_BulletList.end(); itr++) {
		(*itr)->Draw();
	}

	for (Player_MineState* p_mine : P_MineList) {
		p_mine->Draw();
	}

	for (Player_Blackhole* p_blackhole : P_BlackholeList) {
		p_blackhole->Draw();
	}
}