#include "stdafx.h"
#include "Player.h"
#include "Physics/CollisionAttr.h"

using namespace YTEngine;
Player::Player()
{

}


Player::~Player()
{
	//�v���C���[�̍��̂����̂̃��X�g����폜����B
	//m_charaCon.RemoveRigidBoby();
	//DeleteGO(bulletManager);
}

void Player::OnDestroy()
{
	//todo �@���}�b�v������B
	if (g_normalMapSRV != nullptr) {
		g_normalMapSRV->Release();
	}
	G_Player_BulletManager().OnDestroy();
}

bool Player::Start()
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"Assets/modelData/unityChan.cmo");
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
	//���݂̓J�v�Z��
	m_charaCon.Init(
		30.0f,
		player_height,
		m_position
	);
	m_charaCon.GetRigidBody()->GetBody()->setUserIndex(enCollisionAttr_Player);
	G_Player_BulletManager().Start();
	//bulletManager = NewGO<Player_BulletManager>(1, "Player_BulletManager");

	//Unity�����̖@���}�b�v�����[�h�B
	//�t�@�C�������g���āA�e�N�X�`�������[�h���āAShaderResrouceView���쐬����B
	//DirectX::CreateDDSTextureFromFileEx(
	//	g_graphicsEngine->GetD3DDevice(),
	//	L"Assets/modelData/utc_nomal.dds",	//���[�h����e�N�X�`���̃p�X�B
	//	0,
	//	D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
	//	0,
	//	0,
	//	false,
	//	nullptr,
	//	&g_normalMapSRV						//�쐬���ꂽSRV�̃A�h���X�̊i�[��B
	//);
	////���f���ɖ@���}�b�v��ݒ肷��B
	//m_model.SetNormalMap(g_normalMapSRV);

	targetSight.Init(L"Resource/sprite/Normal_TargetSight.dds", 100, 100);
	return true;
}

void Player::Update()
{

	m_moveSpeed.x = 0.0f;
	//m_moveSpeed.y = 0.0f;
	m_moveSpeed.z = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ��󂯎��B
	MoveEffect();

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

	m_moveSpeed += camera_forward * lStick_y *2.0f;//+=��=�ɂ���ƃW�����v���x�ɉe�����y�ڂ��B
	m_moveSpeed += camera_rite * lStick_x*2.0f;
	//�v���C���[�̑O�������v�Z

	//��]
	/*
	�v���C���[�̐i�s�����̊p�x�����Ƃ߂�ɂ́A
	�X�e�B�b�N�̓��͕����̒P�ʃx�N�g���ƃv���C���[�̑O���̒P�ʃx�N�g���̓��ς����߂�B
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
	//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}
	//�v���C���[�̍U���B
	Player_Bullet_Controller();


	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//�ړ��B

																				 //���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());

	//�V���h�E�}�b�v���X�V�B
	Shadow_map().UpdateFromLightTarget(
		{ m_position.x, m_position.y + 1000.0f, m_position.z },
		m_position
	);

	targetSight.Update(m_targetSight_position, CQuaternion::Identity(), { 1.0f,1.0f,1.0f });

}

void Player::MoveEffect()
{
	/*
	�ړ����̍ő呬�x�W���ƍŏ����x�W���B
	*/
	const static float movespeed_MAX = 800.0f;
	const static float movespeed_MIN = 200.0f;
	/*
	�ړ����̑��x�W���B
	*/
	static float movespeed = movespeed_MIN;

	//LB1�{�^���������ĉ������Ă���Ƃ�
	if (g_pad[0].IsPress(enButtonLB1))
	{
		if (movespeed < movespeed_MAX) {
			movespeed += 800.0f;
		}
		m_model.DirectionLight_Red01(4.0f);
	}
	//LB1�{�^���������Ă��炸�A�������Ă��Ȃ��Ƃ�
	else {
		if (movespeed > movespeed_MIN) {
			movespeed -= 20.0f;
		}
		m_model.DirectionLight_ReturnRed(4.0f);
	}
	//���X�e�B�b�N�̓��͗ʂ��󂯎��B
	lStick_x = g_pad[0].GetLStickXF()*movespeed;
	lStick_y = g_pad[0].GetLStickYF()*movespeed;
}

void Player::Player_Bullet_Controller()
{
	//�ˌ��{�^����������Ă��邩����B
	if (g_pad[0].IsPress(enButtonRB2))
	{
		//�ˌ������B
		G_Player_BulletManager().bulletShot(m_position, m_forward);
		//bulletManager->Shot(m_position, m_forward);

		//�v���C���[�𐳖ʂɌ������邽�߂ɁA��]���Œ肷��B
		//player_rotationFlag = false;
	}
	else {
		//��]�̌Œ�������B
		//player_rotationFlag = true;
	}
	G_Player_BulletManager().Update();
}


void Player::Player_Jump()
{
	//��s���Ă��邩�𔻒肷��t���O�B
	static bool FlightFlag = false;
	/*
	��s���O�ɏ㏸�������������𔻒肷��t���O�B
	true�ŏ㏸���Bfalse�ŗ������B
	*/
	static bool Flight_moveSpeedFlag = true;
	//�󒆔�s�B
	//LB1�{�^���������ꂽ�Ƃ�
	if (g_pad[0].IsTrigger(enButtonX))
	{
		//��s���Ă��炸�A�����v���C���[���㏸����������(�n�ʂɂ��Ă��Ȃ�)�Ȃ�
		if ((FlightFlag == false) && (m_moveSpeed.y != 0.0f)) {
			//��s����B
			FlightFlag = true;
			//���O�ɏ㏸�������������𔻒肷��B
			if (m_moveSpeed.y > 0.0f) {
				//�㏸���B
				Flight_moveSpeedFlag = true;
			}
			else {
				//�������B
				Flight_moveSpeedFlag = false;
			}
		}
		else {//��s���Ă����
			  //��s���~�߂�B
			FlightFlag = false;
		}
	}

	if (FlightFlag == false) {
		//���s���B

		//A�{�^���������ꂽ��
		if (g_pad[0].IsPress(enButtonA)) {
			//�W�����v����B
			if (m_moveSpeed.y < 0.0f) {
				m_moveSpeed.y += 350.0f;
			}
			m_moveSpeed.y += 50.0f;	//������ɑ��x��ݒ肵�āA
			m_charaCon.Jump();		//�L�����N�^�[�R���g���[���[�ɃW�����v�������Ƃ�ʒm����B
		}
		else {
			m_moveSpeed.y -= 200.0f;
		}
	}
	else {
		//��s���B
		//���炩�ɋ󒆒�~����B
		if (m_moveSpeed.y != 0.0f) {
			if (Flight_moveSpeedFlag == true) {
				//���O�ɏ㏸���̂Ƃ�
				if (m_moveSpeed.y > 0.0f) {

					m_moveSpeed.y -= 150.0f;
				}
				else {
					m_moveSpeed.y = 0.0f;
				}
			}
			else {
				//���O�ɗ������̂Ƃ�
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
	//�V���h�E�L���X�^�[��o�^�B
	Shadow_map().RegistShadowCaster(&m_model);
}