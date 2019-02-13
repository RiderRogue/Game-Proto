#include "stdafx.h"
#include "Player_BulletManager.h"




Player_BulletManager::Player_BulletManager()
{
}


Player_BulletManager::~Player_BulletManager()
{
}



bool Player_BulletManager::Start()
{
	//cmoファイルの読み込み。
	m_modelproto.Init(L"Assets/modelData/bullet.cmo");
	m_modelproto.SetDirectionLight(5.0f,5.0f,0.0f);
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//flagをfalseにして、発射されていないように初期化する。
		bullet[i].flag = false;
		bullet[i].desflag = false;
		bullet[i].m_model = m_modelproto;
	}
	return true;
}

void Player_BulletManager::Update()
{
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//発射している弾だけ
		if (bullet[i].flag == true) {
			CVector3 m_moveSpeed = CVector3::Zero();
			m_moveSpeed += bullet[i].m_forward*bulletmoveSpeed*100.0f;
			bullet[i].m_position = bullet[i].m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//移動。
			//ワールド行列の更新。
			bullet[i].m_model.UpdateWorldMatrix(bullet[i].m_position, CQuaternion::Identity(), CVector3::One());
			bullet[i].lengthcount+=1;
		}
	}

	for (int i = 0; i < Player_Bullet_NUM; i++) {
		//発射している弾だけ
		if (bullet[i].flag == true) {
			if (bullet[i].m_charaCon.Gethit() == true) {
				bullet[i].desflag = true;
			}
		}
	}

	for (int i = 0; i < Player_Bullet_NUM; i++) {
		if (bullet[i].flag == true) {
			if (bullet[i].lengthcount >= 20) {
				//一定数進んだ際に、死亡フラグをあげる。
				bullet[i].desflag = true;
			}
		}
	}
	for (int i = 0; i < Player_Bullet_NUM; i++) {
		if (bullet[i].desflag == true) {
			//剛体を削除。
			bullet[i].m_charaCon.RemoveRigidBoby();
			//削除されたのでフラグをリセット。
			bullet[i].flag = false;
			bullet[i].desflag = false;
		}
	}

	if (count >= 1) {
		count++;
	}
	if (count >= 5) {
		count = 0;
	}
}

void Player_BulletManager::Shot(const CVector3 p_position, const CVector3 p_forward)
{
	if (count==0) {
		for (int i = 0; i < Player_Bullet_NUM; i++) {
			if (bullet[i].flag == false) {
				bullet[i].flag = true;
				bullet[i].desflag = false;
				bullet[i].m_position = p_position;
				bullet[i].m_forward = p_forward;
				bullet[i].m_position += p_forward*90.0f;
				bullet[i].m_position.y += 90.0f;
				bullet[i].m_charaCon.Init(
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