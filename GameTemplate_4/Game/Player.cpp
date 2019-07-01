#include "stdafx.h"
#include "Player.h"
#include "gauge.h"


#include "Player_BulletManager.h"
#include "Player_MissileManager.h"
#include "GameBase/GameBase.h"
using namespace YTEngine;
Player::Player()
{

}


Player::~Player()
{
	//プレイヤーの剛体を剛体のリストから削除する。
	//m_charaCon.RemoveRigidBoby();
	//DeleteGO(bulletManager);
}

void Player::OnDestroy()
{
	//todo 法線マップを解放。
	if (g_normalMapSRV != nullptr) {
		g_normalMapSRV->Release();
	}
	G_Player_BulletManager().OnDestroy();
	DeleteGO(m_gauge);
}

bool Player::Start()
{
	player_desflag = false;
	HP = HP_MAX;
	Energy = Energy_MAX;

	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/Player_Robo_1.cmo");
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
	m_forward.Normalize();
	//右方向
	m_rite.x = mRot.m[0][0];
	m_rite.y = mRot.m[0][1];
	m_rite.z = mRot.m[0][2];
	m_rite.Normalize();


	
	G_Player_BulletManager().Start();
	//bulletManager = NewGO<Player_BulletManager>(1, "Player_BulletManager");

	//Unityちゃんの法線マップをロード。
	//ファイル名を使って、テクスチャをロードして、ShaderResrouceViewを作成する。
	//DirectX::CreateDDSTextureFromFileEx(
	//	g_graphicsEngine->GetD3DDevice(),
	//	L"Assets/modelData/utc_nomal.dds",	//ロードするテクスチャのパス。
	//	0,
	//	D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
	//	0,
	//	0,
	//	false,
	//	nullptr,
	//	&g_normalMapSRV						//作成されたSRVのアドレスの格納先。
	//);
	////モデルに法線マップを設定する。
	//m_model.SetNormalMap(g_normalMapSRV);

	targetSight.Init(L"Resource/sprite/Normal_TargetSight.dds", 100, 100);
	m_gauge = NewGO<gauge>(0, "Gauge");
	//サウンドエンジンを初期化。
	m_soundEngine.Init();
	m_position_center = { m_position.x,m_position.y + (player_height / 2),m_position.z };

	//ワンショット再生のSE
	m_se[0].Init(L"Assets/sound/landing.wav");
	m_se[1].Init(L"Assets/sound/landing.wav");
	m_se[2].Init(L"Assets/sound/landing.wav");
	m_movese.Init(L"Assets/sound/move.wav");
	m_Jumpse.Init(L"Assets/sound/JumpSE.wav");
	Bullet_vector = m_forward;

	//P_AnimationClips[walk].Load(L"Assets/animData/Player_walk_1.tka");
	P_AnimationClips[idle].Load(L"Assets/animData/Player_idle_1.tka");

	//P_AnimationClips[walk].SetLoopFlag(true);
	P_AnimationClips[idle].SetLoopFlag(true);
	P_Animation.Init(
		m_model,
		P_AnimationClips,
		Animnum
	);
	//P_Animation.Play(idle);
	P_Animation.Update(1.0f/30.0f);
	return true;
}

void Player::Update()
{
	//サウンドエンジンを更新。
	m_soundEngine.Update();

	m_moveSpeed.x = 0.0f;
	//m_moveSpeed.y = 0.0f;
	m_moveSpeed.z = 0.0f;

	m_moveSpeed_side = 0.0f;

	//左スティックの入力量を受け取る。
	MoveEffect();

	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
	m_forward.Normalize();
	//右方向
	m_rite.x = mRot.m[0][0];
	m_rite.y = mRot.m[0][1];
	m_rite.z = mRot.m[0][2];
	m_rite.Normalize();

	Bullet_vector = m_forward;
	m_moveSpeed += camera_forward * lStick_y *2.0f;//+=を=にするとジャンプ速度に影響を及ぼす。
	m_moveSpeed += camera_rite * lStick_x*2.0f;
	//横方向の移動速度を保持する。カメラに使う。
	m_moveSpeed_side = lStick_x*2.0f;
	//プレイヤーの前方向を計算

	//回転
	/*
	プレイヤーの進行方向の角度をもとめるには、
	スティックの入力方向の単位ベクトルとプレイヤーの前方の単位ベクトルの内積を求める。
	*/
	/*CVector3 moveAngle = camera_forward;
	moveAngle.y = 0.0f;
	float player_rot;
	player_rot = moveAngle.Dot(m_forward);
	if (camera_rot_angle>0.0f) {
	player_rot = acos(player_rot);
	}
	else {
	player_rot = -acos(player_rot);
	}*/

	qBias.SetRotationDeg(CVector3::AxisY(), camera_rot_angle *camera_rot_speed);
	m_rotation.Multiply(qBias);

	

	if (m_charaCon.IsOnGround()) {
		//地面についた。
		m_moveSpeed.y = 0.0f;
	}
	//動いているか？
	if ((m_moveSpeed.x==0.0f)&&(m_moveSpeed.z == 0.0f)) 
	{
		P_Animation.Play(idle, 0.25);
	}
	else {
		//P_Animation.Play(walk,0.25);
	}
	Player_Jump();
	//HP管理。
	HPcontrol();
	//エナジー管理。
	Energycontrol();
	//プレイヤーの攻撃。
	Bullet_Missile_Controller();

	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//移動。
	m_position_center = { m_position.x,m_position.y + (player_height / 2),m_position.z };

	 //ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());

	//シャドウマップを更新。
	Shadow_map().UpdateFromLightTarget(
		{ m_position.x, m_position.y + 1000.0f, m_position.z },
		m_position
	);

	targetSight.Update(m_targetSight_position, CQuaternion::Identity(), { 0.5f,0.5f,1.0f });
	P_Animation.Update(1.0f / 30.0f);
	/*if (player_desflag == true) {
		FindGO<GameBase>("GameBase")->ChangeScene(GameBase::GameBase_title);
	}*/
	//if (g_pad[0].IsTrigger(enButtonA)) {
	//	//Aボタンが押されたらSEを鳴らす。
	//	static int m_playSENo = 0;
	//	m_se[m_playSENo].Play(false);
	//	m_playSENo++;
	//	m_playSENo = m_playSENo % 2;
	//}
	//viewport.Project
}

void Player::MoveEffect()
{
	/*
	移動時の最大速度係数と最小速度係数。
	*/
	const static float movespeed_QB = 1800.0f;
	const static float movespeed_MAX = 600.0f;
	const static float movespeed_MIN = 200.0f;
	/*
	移動時の速度係数。
	*/
	static float movespeed = movespeed_MIN;

	//LB1ボタンを押して加速しているとき
	if ((g_pad[0].IsPress(enButtonLB1)) && (EnergyOutFlag == false))
	{
		//ブースト開始から30フレームはクイックブースト。
		if (boostTime <= 10.0f) {
			movespeed = movespeed_QB;
			Energy -= 20.0f;  //エナジー消費
			boostTime++;
		}
		else {
			//クイックブースト終了後は速度を下げる。
			if (movespeed > movespeed_MAX) {
				movespeed -= (movespeed_QB- movespeed_MAX);
				Energy -= 5.0f;  //エナジー消費
			}
			else {
				movespeed = movespeed_MAX;
				Energy -= 5.0f;  //エナジー消費
			}
			
		}
		m_model.DirectionLight_Red01(4.0f);
		MoveFlag = true;
	}
	//LB1ボタンを押しておらず、加速していないとき
	else {
		if (movespeed > movespeed_MIN) {
			movespeed -= 90.0f;
		}
		m_model.DirectionLight_ReturnRed(4.0f);
		MoveFlag = false;
		boostTime = 0.0f;
	}
	//左スティックの入力量を受け取る。
	lStick_x = g_pad[0].GetLStickXF()*movespeed;
	lStick_y = g_pad[0].GetLStickYF()*movespeed;

	if ((lStick_x == 0.0f) && (lStick_y == 0.0f)) {
		m_movese.Stop();
	}
	else {		
		m_movese.Play(true);
	}
}

void Player::Bullet_Missile_Controller()
{
	//ミサイル発射ボタンが押されていたらtrueにする。
	static bool M_Lockonflag = false;
	float kaku_proto = Camera_vector.Dot(m_forward);
	if (kaku_proto >= 1.0f) {
		kaku_proto = 1.0f;
	}
	if (kaku_proto <= -1.0f) {
		kaku_proto = -1.0f;
	}
	float kaku = acosf(kaku_proto);//２つのべクトルの内積のアークコサインを求める。(ラジアン)
	//ここで非数がでている。直す必要あり。
	float degree = CMath::RadToDeg(kaku);
	CQuaternion qBullet = CQuaternion::Identity();

	if (Cameraplus > 0.0f) {
		qBullet.SetRotationDeg(m_rite, (degree));
	}
	else {
		qBullet.SetRotationDeg(m_rite, -(degree));
	}
	
	qBullet.Multiply(Bullet_vector);

	static int bullet = 0;
	static int b_cnt = 0;
	if (g_pad[0].IsTrigger(enButtonB)) {
		bullet++;
		if (bullet >= 2) {
			bullet = 0;
		}
	}
	//射撃ボタンが押されているか判定。
	if (g_pad[0].IsPress(enButtonRB2))
	{
		//射撃処理。
		if (bullet == 0) {
			G_Player_BulletManager().bulletShot(m_position, Bullet_vector);
		}
		else {
			G_Player_BulletManager().BlackholeShot(m_position, Bullet_vector);
		}
	}

	static int expl = 0;//長押しで全て起爆。
	//マインボタンが押されているか判定。
	if (g_pad[0].IsTrigger(enButtonX)) {
		G_Player_BulletManager().mineShot(m_position);
		expl = 0;//長押しの初期化。
	}

	
	if (g_pad[0].IsPress(enButtonX)) {
		expl++;
	}
	if (expl > 30) {
		G_Player_BulletManager().mineexplosion();
		expl = 0;
	}
	G_Player_BulletManager().Update();

	//ミサイルボタンが押されているか判定。
	if (g_pad[0].IsPress(enButtonRB1))
	{
		//射撃処理。
		G_Player_MissileManager().missileLock(m_position, m_forward, camera_forward);
		M_Lockonflag = true;
	}
	else {
		if (M_Lockonflag == true) {
			//ボタンを離したので発射。
			G_Player_MissileManager().missileShot(m_position, m_forward, camera_forward);
			//発射したのでフラグを元に戻す。
			M_Lockonflag = false;
		}
	}
	G_Player_MissileManager().Update();
}

void Player::HPcontrol()
{
	//HP切れ
	if (HP <= 0.0f) {
		HP = 0.0f;
		player_desflag = true;
	}
	m_gauge->HP_meter(HP, HP_MAX);
}

void Player::Energycontrol()
{
	//エナジー切れ
	if (Energy <= 0.0f) {
		Energy = 0.0f;
		EnergyOutFlag = true;
	}

	//加速もジャンプもしていないときはエナジーを回復する。
	if ((MoveFlag == false) && (JumpFlag == false)){
		if (Energy < Energy_MAX) {
			//接地時にエナジー回復
			Energy += 5.0f;
		}
		else {
			Energy = Energy_MAX;
			EnergyOutFlag = false;
		}
	}
	m_gauge->Energy_meter(Energy, Energy_MAX);
}


void Player::Player_Jump()
{
	//飛行中かを格納する。
	static bool flyFlag = false;

	//Aボタンが押されたら
	if ((g_pad[0].IsPress(enButtonA))&&(EnergyOutFlag==false)) {
		//ジャンプする。
		if (m_moveSpeed.y < 0.0f) {
			m_moveSpeed.y += 550.0f;
		}

		//LB1ボタンを押して加速しているとき
		if ((g_pad[0].IsPress(enButtonLB1)) && (EnergyOutFlag == false))
		{
			m_moveSpeed.y += 250.0f;
		}
		else {
			m_moveSpeed.y += 200.0f;	//上方向に速度を設定して、
		}
		
		if (m_charaCon.IsOnGround()) {
			m_moveSpeed.y += 850.0f;
		}
		m_charaCon.Jump();		//キャラクターコントローラーにジャンプしたことを通知する。
		Energy -= 5.0f;  //エナジー消費
		JumpFlag = true;  //ジャンプボタンが押された
		m_Jumpse.Play(true);
	}
	else {
		//重力
		m_moveSpeed.y -= 400.0f;
		JumpFlag = false;
		m_Jumpse.Stop();
	}
	if (!m_charaCon.IsOnGround()) {
		flyFlag = true;
	}

	if (flyFlag==true) {
		//飛行中に着地したら
		if (m_charaCon.IsOnGround()) {
			//効果音(着地音)を鳴らす。
			static int m_playSENo = 0;
			if (m_se[-(m_playSENo - 2)].IsPlaying()) {
				m_se[-(m_playSENo - 2)].Stop();
			}
			m_playSENo++;
			m_playSENo = m_playSENo % 3;
			m_se[m_playSENo].Play(false);
			
			flyFlag = false;
		}
	}
}
void Player::Draw()
{
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
	G_Player_BulletManager().Draw();
}

void Player::PostDraw()
{
	targetSight.Draw();
}

void Player::SetShadowCasters()
{
	//シャドウキャスターを登録。
	Shadow_map().RegistShadowCaster(&m_model);
}