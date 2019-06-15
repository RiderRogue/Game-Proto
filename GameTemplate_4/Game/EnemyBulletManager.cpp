#include "stdafx.h"
#include "EnemyBulletManager.h"
#include "Physics/CollisionAttr.h"

namespace {
	const CVector3 MODEL_FORWARD = CVector3::AxisZ();
}

void Enemy_BulletState::Init(CVector3 eposition, CVector3 eforward) {
	m_position = eposition;
	m_forward = eforward;
	m_position += eforward * 90.0f;
	m_position.y += 10.0f;
	//�G�e�̔��a�B
	float radius = 10.0f;
	m_bulletCon.Init(
		radius,
		m_position
	);
	m_bulletCon.GetRigidBody()->GetBody()->setUserIndex(enCollisionAttr_EnemyBullet);
	m_model.Init(L"Assets/modelData/EnemyBullet.cmo");
	m_model.SetDirectionLight(5.0f, 5.0f, 0.0f);
	m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	lengthcount = 0;
	hitflag = false;
	desflag = false;
	lengthcountMAX = 50;
	bulletmoveSpeed = 2000.0f;
}

void Enemy_BulletState::bulletmove(CVector3 P_pos) {
	//�G�e�̈ړ������B
	CVector3 m_moveSpeed = CVector3::Zero();
	m_moveSpeed += m_forward * bulletmoveSpeed;
	m_position = m_bulletCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//�ړ��B
	/*int in = m_bulletCon.GetRigidBody()->GetBody()->getUserIndex();
	if ( in== enCollisionAttr_EnemyBullet) {
		int i = 0;
	}*/
	 //���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	lengthcount += 1;

	if (m_bulletCon.GetE_bullethit() == true) {
		//�v���C���[�ɏՓ˂����ۂɁA���S�t���O��������B
		hitflag = true;
		desflag = true;
	}
	if (m_bulletCon.Gethit() == true) {
		//�X�e�[�W�ɏՓ˂����ۂɁA���S�t���O��������B
		desflag = true;
	}
	if (lengthcount >= lengthcountMAX) {
		//��苗���i�񂾍ۂɁA���S�t���O��������B
		desflag = true;
	}
}

void Enemy_BulletState::Draw() {
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}


void Enemy_MissileState::Init(CVector3 eposition, CVector3 eforward) {
	m_position = eposition;
	//m_forward = eforward;
	//m_position += eforward * 90.0f;
	m_position.y += 10.0f;
	//�G�e�̔��a�B
	float radius = 15.0f;
	m_bulletCon.Init(
		radius,
		m_position
	);
	m_bulletCon.GetRigidBody()->GetBody()->setUserIndex(enCollisionAttr_EnemyBullet);
	m_model.Init(L"Assets/modelData/EnemyMissile.cmo");

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

	m_model.SetDirectionLight(1.0f, 1.0f, 1.0f);

	CVector3 vDist;
	vDist.Subtract(eforward, m_position);
	vDist.Normalize();
	//���ςłł�cos�Ƃ��p�x�ɕς���B
	float degree = CMath::RadToDeg(acosf(eforward.Dot(m_forward)));
	if (degree >= 0.1f) {
		CVector3 rotAxis;
		rotAxis.Cross(m_forward, eforward);
		rotAxis.Normalize();
		//��]�B
		CQuaternion qBias;
		qBias.SetRotationDeg(rotAxis, degree);
		m_rotation.Multiply(qBias);
	}

	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
	lengthcount = 0;
	hitflag = false;
	desflag = false;
	lengthcountMAX = 300;
	bulletmoveSpeed = 700.0f;
}

void Enemy_MissileState::bulletmove(CVector3 P_pos) {

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
	CVector3 vDist;	
	vDist.Subtract(P_pos, m_position);
	vDist.Normalize();
	vDist += m_forward*10.0f;//���炷�B
	vDist.Normalize();

	
	//���ςłł�cos�Ƃ��p�x�ɕς���B
	float degree = CMath::RadToDeg(acosf(vDist.Dot(MODEL_FORWARD)));
	if (degree>=0.0001f) {
		CVector3 rotAxis;
		rotAxis.Cross(MODEL_FORWARD,vDist);
		rotAxis.Normalize();
		//��]�B
		m_rotation.SetRotationDeg(rotAxis, degree);
	}
	//�G�e�̈ړ������B
	CVector3 m_moveSpeed = CVector3::Zero();
	m_moveSpeed += vDist * bulletmoveSpeed;

	m_position = m_bulletCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//�ړ��B

	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
	lengthcount += 1;

	if (m_bulletCon.GetE_bullethit() == true) {
		//�v���C���[�ɏՓ˂����ۂɁA���S�t���O��������B
		hitflag = true;
		desflag = true;
	}
	if (m_bulletCon.Gethit() == true) {
		//�X�e�[�W�ɏՓ˂����ۂɁA���S�t���O��������B
		desflag = true;
	}
	if (lengthcount >= lengthcountMAX) {
		//��苗���i�񂾍ۂɁA���S�t���O��������B
		desflag = true;
	}
}

EnemyBulletManager::EnemyBulletManager()
{
}

EnemyBulletManager::~EnemyBulletManager()
{
}

void EnemyBulletManager::Start()
{
	damage = 10.0f;
	player = FindGO<Player>("Player");
}

void EnemyBulletManager::Update()
{
	for (std::list<Enemy_BulletState*>::iterator itr = E_BulletList.begin(); itr != E_BulletList.end(); itr++) {
		(*itr)->bulletmove(player->GetPosition_center());//�ǔ���̍��W�B
		//�v���C���[�Ƃ̓����蔻������̂łƂ�B
		if ((*itr)->Gethitflag() == true)
		{
			player->Damage(damage);
		}
	}
	erasebullet();
}

void EnemyBulletManager::Draw()
{
	for (std::list<Enemy_BulletState*>::iterator itr = E_BulletList.begin(); itr != E_BulletList.end(); itr++) {
		(*itr)->Draw();
	}
}

void EnemyBulletManager::OnDestroy()
{
	erasebullet_All();
}

void EnemyBulletManager::bulletShot(const CVector3 e_position, const CVector3 e_forward)
{
	Enemy_BulletState* bullet;
	bullet = new(Enemy_BulletState);
	bullet->Init(e_position, e_forward);
	E_BulletList.push_back(bullet);
}

void EnemyBulletManager::MissileShot(const CVector3 e_position, const CVector3 e_forward)
{
	Enemy_MissileState* bullet;
	bullet = new(Enemy_MissileState);
	bullet->Init(e_position, e_forward);
	E_BulletList.push_back(bullet);
}

void EnemyBulletManager::erasebullet()
{
	for (std::list<Enemy_BulletState*>::iterator itr = E_BulletList.begin(); itr != E_BulletList.end(); ) {
		if ((*itr)->Getdesflag() == true) {
			//���S���Ă���΍폜����B
			delete(*itr);
			itr = E_BulletList.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void EnemyBulletManager::erasebullet_All() 
{
	for (std::list<Enemy_BulletState*>::iterator itr = E_BulletList.begin(); itr != E_BulletList.end(); ) {
		delete(*itr);
		itr = E_BulletList.erase(itr);
	}
}
