#include "stdafx.h"
#include "EnemyBulletManager.h"
#include "Physics/CollisionAttr.h"

void Enemy_BulletState::Init(CVector3 eposition, CVector3 eforward) {
	m_position = eposition;
	m_forward = eforward;
	m_position += eforward * 90.0f;
	m_position.y += 10.0f;
	//敵弾の半径。
	float radius = 10.0f;
	m_bulletCon.Init(
		radius,
		m_position
	);
	m_bulletCon.GetRigidBody()->GetBody()->setUserIndex(enCollisionAttr_EnemyBullet);
	m_model.Init(L"Assets/modelData/EnemyBullet.cmo");
	m_model.SetDirectionLight(5.0f, 5.0f, 0.0f);
	lengthcount = 0;
	hitflag = false;
	desflag = false;
}

void Enemy_BulletState::bulletmove(float bulletmoveSpeed,int lengthcountMAX) {
	//敵弾の移動処理。
	CVector3 m_moveSpeed = CVector3::Zero();
	m_moveSpeed += m_forward * bulletmoveSpeed;
	m_position = m_bulletCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//移動。
	int in = m_bulletCon.GetRigidBody()->GetBody()->getUserIndex();
	if ( in== enCollisionAttr_EnemyBullet) {
		int i = 0;
	}
	 //ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	lengthcount += 1;

	if (m_bulletCon.GetE_bullethit() == true) {
		//プレイヤーに衝突した際に、死亡フラグをあげる。
		hitflag = true;
		desflag = true;
	}
	if (m_bulletCon.Gethit() == true) {
		//ステージに衝突した際に、死亡フラグをあげる。
		desflag = true;
	}
	if (lengthcount >= lengthcountMAX) {
		//一定距離進んだ際に、死亡フラグをあげる。
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
		(*itr)->bulletmove(bulletmoveSpeed, lengthcountMAX);
		//プレイヤーとの当たり判定を剛体でとる。
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

void EnemyBulletManager::erasebullet()
{
	for (std::list<Enemy_BulletState*>::iterator itr = E_BulletList.begin(); itr != E_BulletList.end(); ) {
		if ((*itr)->Getdesflag() == true) {
			//死亡していれば削除する。
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
