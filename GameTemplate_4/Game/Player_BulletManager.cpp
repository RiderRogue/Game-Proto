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
		//発射している弾だけ
		if (bullet[i].flag == true) {
			//剛体を削除。
			bullet[i].m_bulletCon.RemoveRigidBoby();
		}
	}

	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		//発射している弾だけ
		if (blackhole[i].flag == true) {
			//剛体を削除。
			blackhole[i].m_bulletCon.RemoveRigidBoby();
		}
	}

	//エフェクトを破棄。
	if (m_sampleEffect != nullptr) {
		m_sampleEffect->Release();
	}

	//エフェクトを破棄。
	if (m_blackholeEffect != nullptr) {
		m_blackholeEffect->Release();
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

	for (int i = 0; i < Player_Mine_NUM; i++) {
		mine[i].flag = false;
		mine[i].desflag = false;
		mine[i].explosion = false;
		mine[i].m_model.Init(L"Assets/modelData/Mine.cmo");
		mine[i].blastrange = 600.0f;//爆風の範囲を設定。
	}

	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		//flagをfalseにして、発射されていないように初期化する。
		blackhole[i].flag = false;
		blackhole[i].desflag = false;
		blackhole[i].time = 0.0f;
		blackhole[i].m_model.Init(L"Assets/modelData/hole.cmo");
		blackhole[i].m_model.SetDirectionLight(5.0f, 5.0f, 0.0f);
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

	//サンプルのエフェクトをロードする。
	m_sampleEffect = Effekseer::Effect::Create(
		G_EffekseerManager().GetManager(),
		(const EFK_CHAR*)L"Assets/effect/black hole.efk",
		20.0f
	);

	//ブラックホールのエフェクトをロードする。
	m_blackholeEffect = Effekseer::Effect::Create(
		G_EffekseerManager().GetManager(),
		(const EFK_CHAR*)L"Assets/effect/black hole.efk",
		20.0f
	);
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

	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		//発射しているブラックホール弾だけ
		if (blackhole[i].flag == true) {
			//ブラックホール弾。
			Blackhole_move(i);
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

	//ブラックホール弾がステージと接触したときに死亡フラグをあげる。
	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		//発射している弾だけ
		if (blackhole[i].flag == true) {
			if (blackhole[i].m_bulletCon.Gethit() == true) {
				//ステージに衝突した際に、爆発フラグをあげる。
				if (blackhole[i].explosion!=true) {
					G_EffekseerManager().Play(m_blackholeEffect, blackhole[i].m_position);
					blackhole[i].explosion = true;
				}			
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

	//ブラックホール弾が発射されて一定距離進んだときに死亡フラグをあげる。
	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		if (blackhole[i].flag == true) {
			if (blackhole[i].lengthcount >= 30) {
				//ステージに衝突した際に、爆発フラグをあげる。
				if (blackhole[i].explosion != true) {
					G_EffekseerManager().Play(m_blackholeEffect, blackhole[i].m_position);
					blackhole[i].explosion = true;
				}
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

	//死亡フラグがたったマインを削除する。
	for (int i = 0; i < Player_Mine_NUM; i++) {
		if (mine[i].desflag == true) {
			
			//再生。
			m_mineEffectHandle = G_EffekseerManager().Play(m_sampleEffect, mine[i].m_position);
			//削除されたのでフラグをリセット。
			mine[i].flag = false;
			mine[i].desflag = false;
		}
	}

	//死亡フラグがたったブラックホール弾を削除する。
	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		if (blackhole[i].desflag == true) {
			//剛体を削除。
			blackhole[i].m_bulletCon.RemoveRigidBoby();
			//削除されたのでフラグをリセット。
			blackhole[i].flag = false;
			blackhole[i].desflag = false;
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
				mine[i].explosion = false;//爆発フラグを初期化。
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
				blackhole[i].explosion = false;//爆発フラグを初期化。
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
								   //敵と衝突した(距離が90.0f以下なら)ら
			if (len <= 90.0f) {
				//当たった弾の死亡フラグをあげる。
				bullet[i].desflag = true;
				//敵と弾の距離を測り、当たっていればダメージ。
				enemy->enemyDamage(BulletDamage);
			}
		}
	}
}

void Player_BulletManager::mineexplosion() {
	for (int i = 0; i < Player_Mine_NUM; i++) {
		//設置しているマインだけ
		if (mine[i].flag == true) {
			//当たった弾の死亡フラグをあげる。
			mine[i].explosion = true;
			mine[i].desflag = true;
		}
	}
}

void Player_BulletManager::EnemyHitMine(Enemy* enemy)
{
	for (int i = 0; i < Player_Mine_NUM; i++) {
		//設置しているマインだけ
		if (mine[i].flag == true) {
			CVector3 v = mine[i].m_position - enemy->GetPosition_center();
			float len = v.Length();//長さ
								   //敵と衝突した(距離が200.0f以下なら)ら
			if ((mine[i].explosion == true) && (len <= mine[i].blastrange)) {
				//敵と弾の距離を測り、当たっていればダメージ。
				enemy->enemyDamage(MineDamage);
			}
			else if (len <= 200.0f) {
				//当たった弾の死亡フラグをあげる。
				mine[i].explosion = true;
				mine[i].desflag = true;
				//敵と弾の距離を測り、当たっていればダメージ。
				enemy->enemyDamage(MineDamage);
			}
		}
	}
}

void Player_BulletManager::EnemyHitBlackhole(Enemy* enemy)
{
	for (int i = 0; i < Player_Blackhole_NUM; i++) {
		//発射しているブラックホールだけ
		if (blackhole[i].flag == true) {
			CVector3 v = blackhole[i].m_position - enemy->GetPosition_center();
			float len = v.Length();//長さ
								   //敵と衝突した(距離が吸収範囲以下なら)ら
			if ((blackhole[i].explosion == true) && (len <= blackholeAbsorb)) {
				//敵と弾の距離を測り、当たっていればダメージ。
				enemy->enemyDamage_Blackhole(BlackholeDamage, blackhole[i].m_position);
				
			}
			else if ((blackhole[i].explosion==false)&&(len <= 90.0f)) {
				//当たった弾の爆発フラグをあげて、エフェクトを再生する。
				blackhole[i].explosion = true;
				G_EffekseerManager().Play(m_blackholeEffect, blackhole[i].m_position);
				//敵と弾の距離を測り、当たっていればダメージ。
				enemy->enemyDamage_Blackhole(BlackholeDamage, blackhole[i].m_position);
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

void Player_BulletManager::Blackhole_move(int bulletNumber)
{
	//爆発していなかったら進む。
	if (blackhole[bulletNumber].explosion != true) {
		CVector3 m_moveSpeed = CVector3::Zero();
		m_moveSpeed += blackhole[bulletNumber].m_forward*bulletmoveSpeed / 20;
		blackhole[bulletNumber].m_position = blackhole[bulletNumber].m_bulletCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//移動。
																																//ワールド行列の更新。
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
			//生成時間を過ぎたら死亡フラグをあげる。
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