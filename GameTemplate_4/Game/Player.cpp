#include "stdafx.h"
#include "Player.h"
#include "Physics/CollisionAttr.h"

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
}

bool Player::Start()
{
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/unityChan.cmo");
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
	//現在はカプセル
	m_charaCon.Init(
		30.0f,
		player_height,
		m_position
	);
	m_charaCon.GetRigidBody()->GetBody()->setUserIndex(enCollisionAttr_Player);
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
	return true;
}

void Player::Update()
{

	m_moveSpeed.x = 0.0f;
	//m_moveSpeed.y = 0.0f;
	m_moveSpeed.z = 0.0f;

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

	m_moveSpeed += camera_forward * lStick_y *2.0f;//+=を=にするとジャンプ速度に影響を及ぼす。
	m_moveSpeed += camera_rite * lStick_x*2.0f;
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

	Player_Jump();
	//キャラクターコントローラーを使用して、座標を更新。
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		m_moveSpeed.y = 0.0f;
	}
	//プレイヤーの攻撃。
	Player_Bullet_Controller();


	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//移動。

																				 //ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());

	//シャドウマップを更新。
	Shadow_map().UpdateFromLightTarget(
		{ m_position.x, m_position.y + 1000.0f, m_position.z },
		m_position
	);

	targetSight.Update(m_targetSight_position, CQuaternion::Identity(), { 1.0f,1.0f,1.0f });

}

void Player::MoveEffect()
{
	/*
	移動時の最大速度係数と最小速度係数。
	*/
	const static float movespeed_MAX = 800.0f;
	const static float movespeed_MIN = 200.0f;
	/*
	移動時の速度係数。
	*/
	static float movespeed = movespeed_MIN;

	//LB1ボタンを押して加速しているとき
	if (g_pad[0].IsPress(enButtonLB1))
	{
		if (movespeed < movespeed_MAX) {
			movespeed += 800.0f;
		}
		m_model.DirectionLight_Red01(4.0f);
	}
	//LB1ボタンを押しておらず、加速していないとき
	else {
		if (movespeed > movespeed_MIN) {
			movespeed -= 20.0f;
		}
		m_model.DirectionLight_ReturnRed(4.0f);
	}
	//左スティックの入力量を受け取る。
	lStick_x = g_pad[0].GetLStickXF()*movespeed;
	lStick_y = g_pad[0].GetLStickYF()*movespeed;
}

void Player::Player_Bullet_Controller()
{
	//射撃ボタンが押されているか判定。
	if (g_pad[0].IsPress(enButtonRB2))
	{
		//射撃処理。
		G_Player_BulletManager().bulletShot(m_position, m_forward);
		//bulletManager->Shot(m_position, m_forward);

		//プレイヤーを正面に向かせるために、回転を固定する。
		//player_rotationFlag = false;
	}
	else {
		//回転の固定を解除。
		//player_rotationFlag = true;
	}
	G_Player_BulletManager().Update();
}


void Player::Player_Jump()
{
	//飛行しているかを判定するフラグ。
	static bool FlightFlag = false;
	/*
	飛行直前に上昇中か落下中かを判定するフラグ。
	trueで上昇中。falseで落下中。
	*/
	static bool Flight_moveSpeedFlag = true;
	//空中飛行。
	//LB1ボタンが押されたとき
	if (g_pad[0].IsTrigger(enButtonX))
	{
		//飛行しておらず、尚且つプレイヤーが上昇中か落下中(地面についていない)なら
		if ((FlightFlag == false) && (m_moveSpeed.y != 0.0f)) {
			//飛行する。
			FlightFlag = true;
			//直前に上昇中か落下中かを判定する。
			if (m_moveSpeed.y > 0.0f) {
				//上昇中。
				Flight_moveSpeedFlag = true;
			}
			else {
				//落下中。
				Flight_moveSpeedFlag = false;
			}
		}
		else {//飛行していれば
			  //飛行を止める。
			FlightFlag = false;
		}
	}

	if (FlightFlag == false) {
		//非飛行時。

		//Aボタンが押されたら
		if (g_pad[0].IsPress(enButtonA)) {
			//ジャンプする。
			if (m_moveSpeed.y < 0.0f) {
				m_moveSpeed.y += 350.0f;
			}
			m_moveSpeed.y += 50.0f;	//上方向に速度を設定して、
			m_charaCon.Jump();		//キャラクターコントローラーにジャンプしたことを通知する。
		}
		else {
			m_moveSpeed.y -= 200.0f;
		}
	}
	else {
		//飛行時。
		//滑らかに空中停止する。
		if (m_moveSpeed.y != 0.0f) {
			if (Flight_moveSpeedFlag == true) {
				//直前に上昇中のとき
				if (m_moveSpeed.y > 0.0f) {

					m_moveSpeed.y -= 150.0f;
				}
				else {
					m_moveSpeed.y = 0.0f;
				}
			}
			else {
				//直前に落下中のとき
				if (m_moveSpeed.y < 0.0f) {

					m_moveSpeed.y += 350.0f;

				}
				else {
					m_moveSpeed.y = 0.0f;
				}
			}
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