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
		//発射している弾だけ
		if (bullet[i].flag == true) {
			//剛体を削除。
			bullet[i].m_bulletCon.RemoveRigidBoby();
		}
	}
}



void Player_BulletManager::Start()
{
	//cmoファイルの読み込み。
	//m_modelproto.Init(L"Assets/modelData/bullet.cmo");
	//m_modelproto.SetDirectionLight(5.0f,5.0f,0.0f);
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//flagをfalseにして、発射されていないように初期化する。
		bullet[i].flag = false;
		bullet[i].desflag = false;
		bullet[i].m_model.Init(L"Assets/modelData/bullet.cmo");
		//bullet[i].m_model.Init(L"Assets/modelData/EnemyBullet.cmo");
		bullet[i].m_model.SetDirectionLight(5.0f, 5.0f, 0.0f);
	}

	for (int i = 0; i < Player_Missile_NUM; i++) {
		//flagをfalseにして、発射されていないように初期化する。
		missile[i].flag = false;
		missile[i].desflag = false;
		missile[i].m_model.Init(L"Assets/modelData/bullet.cmo");
		missile[i].m_model.SetDirectionLight(5.0f, 5.0f, 0.0f);
		//ロックオン番号の初期化。
		missile[i].lockon_enemyNumber = -1;
	}
	//ミサイルアモ。今回は55発。
	missile_ammo_NUM = 55;
}

void Player_BulletManager::Update()
{
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//発射している弾だけ
		if (bullet[i].flag == true) {
			//通常弾。
			Normalbullet_move(i);
		}
	}

	for (int i = 0; i < Player_Missile_NUM; i++) {
		//発射している弾だけ
		if (missile[i].flag == true) {
			//通常弾。
			missile_move(i);
		}
	}
	//弾がステージと接触したときに死亡フラグをあげる。
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//発射している弾だけ
		if (bullet[i].flag == true) {
			if (bullet[i].m_bulletCon.Gethit() == true) {
				//ステージに衝突した際に、死亡フラグをあげる。
				bullet[i].desflag = true;
			}
		}
	}

	//ミサイルがステージと接触したときに死亡フラグをあげる。
	for (int i = 0; i < Player_Missile_NUM; i++) {
		//発射している弾だけ
		if (missile[i].flag == true) {
			if (missile[i].m_bulletCon.Gethit() == true) {
				//ステージに衝突した際に、死亡フラグをあげる。
				missile[i].desflag = true;
			}
		}
	}
	//弾が発射されて一定距離進んだときに死亡フラグをあげる。
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		if (bullet[i].flag == true) {
			if (bullet[i].lengthcount >= 20) {
				//一定距離進んだ際に、死亡フラグをあげる。
				bullet[i].desflag = true;
			}
		}
	}

	//ミサイルが発射されて一定距離進んだときに死亡フラグをあげる。
	for (int i = 0; i < Player_Missile_NUM; i++) {
		if (missile[i].flag == true) {
			if (missile[i].lengthcount >= 300) {
				//一定距離進んだ際に、死亡フラグをあげる。
				missile[i].desflag = true;
			}
		}
	}
	//死亡フラグがたった弾を削除する。
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		if (bullet[i].desflag == true) {
			//剛体を削除。
			bullet[i].m_bulletCon.RemoveRigidBoby();
			//削除されたのでフラグをリセット。
			bullet[i].flag = false;
			bullet[i].desflag = false;
		}
	}

	//死亡フラグがたったミサイルを削除する。
	for (int i = 0; i < Player_Missile_NUM; i++) {
		if (missile[i].desflag == true) {
			//剛体を削除。
			missile[i].m_bulletCon.RemoveRigidBoby();
			//削除されたのでフラグをリセット。
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
	//弾切れなら発射しない。
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
		//発射している弾だけ
		if (bullet[i].flag == true) {
			CVector3 v = bullet[i].m_position - enemy->GetPosition_center();
			float len = v.Length();//長さ
								   //敵と衝突した(距離が30.0f以下なら)ら
			if (len <= 90.0f) {
				//当たった弾の死亡フラグをあげる。
				bullet[i].desflag = true;
				//敵と弾の距離を測り、当たっていればダメージ。
				enemy->enemyDamage(BulletDamage);
			}
		}
	}
}

void Player_BulletManager::Normalbullet_move(int bulletNumber)
{
	CVector3 m_moveSpeed = CVector3::Zero();
	m_moveSpeed += bullet[bulletNumber].m_forward*bulletmoveSpeed;
	bullet[bulletNumber].m_position = bullet[bulletNumber].m_bulletCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//移動。
																															//ワールド行列の更新。
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