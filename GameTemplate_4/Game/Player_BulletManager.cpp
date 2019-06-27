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
	m_position = m_bulletCon.ReflectExecute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//移動。
	//m_forward = m_moveSpeed;//移動ベクトルの変更。
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	lengthcount += 1;

	//弾がステージと接触したときに死亡フラグをあげる。
	if (m_bulletCon.Gethit() == true) {
		//ステージに衝突した際に、死亡フラグをあげる。
		//desflag = true;
	}
	//弾が発射されて一定距離進んだときに死亡フラグをあげる。
	if (lengthcount >= lengthcount_MAX) {
		//一定距離進んだ際に、死亡フラグをあげる。
		desflag = true;
	}
}
void Player_BulletState::Hitbullet(Enemy* enemy)
{
	CVector3 v = m_position - enemy->GetPosition_center();
	float len = v.Length();//長さ
						   //敵と衝突した(距離が90.0f以下なら)ら
	if (len <= 90.0f) {
		//当たった弾の死亡フラグをあげる。
		desflag = true;
		//敵と弾の距離を測り、当たっていればダメージ。
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
	explosion = false;//爆発フラグを初期化。
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
	float len = v.Length();//長さ
	 //敵と衝突した(距離が200.0f以下なら)ら
	if ((explosion == true) && (len <= blastrange)) {
		//敵と弾の距離を測り、当たっていればダメージ。
		enemy->enemyDamage(MineDamage);
	}
	else if (len <= (blastrange/3)) {
		//当たった弾の死亡フラグをあげる。
		explosion = true;
		desflag = true;
		//敵と弾の距離を測り、当たっていればダメージ。
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
	explosion = false;//爆発フラグを初期化。
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
	//爆発していなかったら進む。
	if (explosion != true) {
		CVector3 m_moveSpeed = CVector3::Zero();
		m_moveSpeed += m_forward*bulletmoveSpeed / 20;
		m_position = m_bulletCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//移動。
																																	  //ワールド行列の更新。
		m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
		lengthcount += 1;
	}
	else {
		//時間経過で拡大する。
		float holesc = 1.0f + time / 5.0f;
		if (holesc >= 6.5f) {
			holesc = 6.5f;
		}
		m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), { holesc,holesc,holesc });
		time++;
		if (time >= blackholeTime) {
			//生成時間を過ぎたら死亡フラグをあげる。
			desflag = true;
		}
	}

	if (m_bulletCon.Gethit() == true) {
		//ステージに衝突した際に、爆発フラグをあげる。
		if (explosion != true) {
			G_EffekseerManager().Play(m_blackholeEffect, m_position);
			explosion = true;
		}
	}

	if (lengthcount >= lengthcount_MAX) {
		//ステージに衝突した際に、爆発フラグをあげる。
		if (explosion != true) {
			G_EffekseerManager().Play(m_blackholeEffect, m_position);
			explosion = true;
		}
	}
}

void Player_Blackhole::HitBlackhole(Enemy* enemy, Effekseer::Effect* m_blackholeEffect)
{
	CVector3 v = m_position - enemy->GetPosition_center();
	float len = v.Length();//長さ
	 //敵と衝突した(距離が吸収範囲以下なら)ら
	if ((explosion == true) && (len <= blackholeAbsorb)) {
		//敵と弾の距離を測り、当たっていればダメージ。
		enemy->enemyDamage_Blackhole(BlackholeDamage, m_position);

	}
	else if ((explosion == false) && (len <= 90.0f)) {
		//当たった弾の爆発フラグをあげて、エフェクトを再生する。
		explosion = true;
		G_EffekseerManager().Play(m_blackholeEffect, m_position);
		//敵と弾の距離を測り、当たっていればダメージ。
		enemy->enemyDamage_Blackhole(BlackholeDamage, m_position);
	}
}
void Player_Blackhole::Draw()
{
	if (explosion != true) {//ブラックホールが生成されていなかったら描画。
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
	float len = Absorb.Length();//長さ
	//ブラックホールが生成され、吸収範囲なら
	if ((explosion == true) && (len <= blackholeAbsorb)) {
		//吸収する。
		Absorb.Normalize();
		Absorb *= 2000.0f;
	}
	else {
		//吸収されない。
		Absorb = CVector3::Zero();
	}
	return Absorb;
}


void Player_BulletManager::OnDestroy()
{
	erasebullet_All();

	//エフェクトを破棄。
	if (m_blackholeEffect != nullptr) {
		m_blackholeEffect->Release();
	}
}



void Player_BulletManager::Start()
{

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

	//ブラックホールのエフェクトをロードする。
	m_blackholeEffect = Effekseer::Effect::Create(
		G_EffekseerManager().GetManager(),
		(const EFK_CHAR*)L"Assets/effect/black hole.efk",
		20.0f
	);
}

void Player_BulletManager::Update()
{
	for (std::list<Player_BulletState*>::iterator itr = P_BulletList.begin(); itr != P_BulletList.end(); itr++) {
		(*itr)->bulletmove();//追尾先の座標。
	}

	for (Player_Blackhole* p_black : P_BlackholeList) {
		p_black->bulletmove(m_blackholeEffect);
	}

	for (int i = 0; i < Player_Missile_NUM; i++) {
		//発射している弾だけ
		if (missile[i].flag == true) {
			//通常弾。
			missile_move(i);
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

	
	//ミサイルが発射されて一定距離進んだときに死亡フラグをあげる。
	for (int i = 0; i < Player_Missile_NUM; i++) {
		if (missile[i].flag == true) {
			if (missile[i].lengthcount >= 300) {
				//一定距離進んだ際に、死亡フラグをあげる。
				missile[i].desflag = true;
			}
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
	if (blackholecount >= 300) {
		blackholecount = 0;
	}
	erasebullet();//不要な弾の削除。
}

void Player_BulletManager::bulletShot(const CVector3 p_position, const CVector3 p_forward)
{
	if (count == 0) {//連射防止
		Player_BulletState* bullet;
		bullet = new(Player_BulletState);
		bullet->Init(p_position, p_forward);
		P_BulletList.push_back(bullet);
		count = 1;
	}
}

void Player_BulletManager::mineShot(const CVector3 p_position)
{
	if (minecount == 0) {//連射防止
		Player_MineState* mine;
		mine = new(Player_MineState);
		mine->Init(p_position);
		P_MineList.push_back(mine);
		minecount = 1;
	}
}

void Player_BulletManager::BlackholeShot(const CVector3 p_position, const CVector3 p_forward)
{
	if (blackholecount == 0) {//連射防止
		Player_Blackhole* blackhole;
		blackhole = new(Player_Blackhole);
		blackhole->Init(p_position, p_forward);
		P_BlackholeList.push_back(blackhole);
		blackholecount = 1;
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
	for (Player_BulletState* p_bullet : P_BulletList) {
		p_bullet->Hitbullet(enemy);
	}
}

void Player_BulletManager::mineexplosion() {
	//全起爆。
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
			//死亡していれば削除する。
			delete(*itr);
			itr = P_BulletList.erase(itr);
		}
		else {
			itr++;
		}
	}

	for (std::list<Player_MineState*>::iterator itr = P_MineList.begin(); itr != P_MineList.end(); ) {
		if ((*itr)->Getdesflag() == true) {
			//死亡していれば削除する。
			delete(*itr);
			itr = P_MineList.erase(itr);
		}
		else {
			itr++;
		}
	}

	for (std::list<Player_Blackhole*>::iterator itr = P_BlackholeList.begin(); itr != P_BlackholeList.end(); ) {
		if ((*itr)->Getdesflag() == true) {
			//死亡していれば削除する。
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