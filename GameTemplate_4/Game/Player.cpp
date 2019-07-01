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
	DeleteGO(m_gauge);
}

bool Player::Start()
{
	player_desflag = false;
	HP = HP_MAX;
	Energy = Energy_MAX;

	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"Assets/modelData/Player_Robo_1.cmo");
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
	m_gauge = NewGO<gauge>(0, "Gauge");
	//�T�E���h�G���W�����������B
	m_soundEngine.Init();
	m_position_center = { m_position.x,m_position.y + (player_height / 2),m_position.z };

	//�����V���b�g�Đ���SE
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
	//�T�E���h�G���W�����X�V�B
	m_soundEngine.Update();

	m_moveSpeed.x = 0.0f;
	//m_moveSpeed.y = 0.0f;
	m_moveSpeed.z = 0.0f;

	m_moveSpeed_side = 0.0f;

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

	Bullet_vector = m_forward;
	m_moveSpeed += camera_forward * lStick_y *2.0f;//+=��=�ɂ���ƃW�����v���x�ɉe�����y�ڂ��B
	m_moveSpeed += camera_rite * lStick_x*2.0f;
	//�������̈ړ����x��ێ�����B�J�����Ɏg���B
	m_moveSpeed_side = lStick_x*2.0f;
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

	

	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}
	//�����Ă��邩�H
	if ((m_moveSpeed.x==0.0f)&&(m_moveSpeed.z == 0.0f)) 
	{
		P_Animation.Play(idle, 0.25);
	}
	else {
		//P_Animation.Play(walk,0.25);
	}
	Player_Jump();
	//HP�Ǘ��B
	HPcontrol();
	//�G�i�W�[�Ǘ��B
	Energycontrol();
	//�v���C���[�̍U���B
	Bullet_Missile_Controller();

	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);//�ړ��B
	m_position_center = { m_position.x,m_position.y + (player_height / 2),m_position.z };

	 //���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());

	//�V���h�E�}�b�v���X�V�B
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
	//	//A�{�^���������ꂽ��SE��炷�B
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
	�ړ����̍ő呬�x�W���ƍŏ����x�W���B
	*/
	const static float movespeed_QB = 1800.0f;
	const static float movespeed_MAX = 600.0f;
	const static float movespeed_MIN = 200.0f;
	/*
	�ړ����̑��x�W���B
	*/
	static float movespeed = movespeed_MIN;

	//LB1�{�^���������ĉ������Ă���Ƃ�
	if ((g_pad[0].IsPress(enButtonLB1)) && (EnergyOutFlag == false))
	{
		//�u�[�X�g�J�n����30�t���[���̓N�C�b�N�u�[�X�g�B
		if (boostTime <= 10.0f) {
			movespeed = movespeed_QB;
			Energy -= 20.0f;  //�G�i�W�[����
			boostTime++;
		}
		else {
			//�N�C�b�N�u�[�X�g�I����͑��x��������B
			if (movespeed > movespeed_MAX) {
				movespeed -= (movespeed_QB- movespeed_MAX);
				Energy -= 5.0f;  //�G�i�W�[����
			}
			else {
				movespeed = movespeed_MAX;
				Energy -= 5.0f;  //�G�i�W�[����
			}
			
		}
		m_model.DirectionLight_Red01(4.0f);
		MoveFlag = true;
	}
	//LB1�{�^���������Ă��炸�A�������Ă��Ȃ��Ƃ�
	else {
		if (movespeed > movespeed_MIN) {
			movespeed -= 90.0f;
		}
		m_model.DirectionLight_ReturnRed(4.0f);
		MoveFlag = false;
		boostTime = 0.0f;
	}
	//���X�e�B�b�N�̓��͗ʂ��󂯎��B
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
	//�~�T�C�����˃{�^����������Ă�����true�ɂ���B
	static bool M_Lockonflag = false;
	float kaku_proto = Camera_vector.Dot(m_forward);
	if (kaku_proto >= 1.0f) {
		kaku_proto = 1.0f;
	}
	if (kaku_proto <= -1.0f) {
		kaku_proto = -1.0f;
	}
	float kaku = acosf(kaku_proto);//�Q�̂׃N�g���̓��ς̃A�[�N�R�T�C�������߂�B(���W�A��)
	//�����Ŕ񐔂��łĂ���B�����K�v����B
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
	//�ˌ��{�^����������Ă��邩����B
	if (g_pad[0].IsPress(enButtonRB2))
	{
		//�ˌ������B
		if (bullet == 0) {
			G_Player_BulletManager().bulletShot(m_position, Bullet_vector);
		}
		else {
			G_Player_BulletManager().BlackholeShot(m_position, Bullet_vector);
		}
	}

	static int expl = 0;//�������őS�ċN���B
	//�}�C���{�^����������Ă��邩����B
	if (g_pad[0].IsTrigger(enButtonX)) {
		G_Player_BulletManager().mineShot(m_position);
		expl = 0;//�������̏������B
	}

	
	if (g_pad[0].IsPress(enButtonX)) {
		expl++;
	}
	if (expl > 30) {
		G_Player_BulletManager().mineexplosion();
		expl = 0;
	}
	G_Player_BulletManager().Update();

	//�~�T�C���{�^����������Ă��邩����B
	if (g_pad[0].IsPress(enButtonRB1))
	{
		//�ˌ������B
		G_Player_MissileManager().missileLock(m_position, m_forward, camera_forward);
		M_Lockonflag = true;
	}
	else {
		if (M_Lockonflag == true) {
			//�{�^���𗣂����̂Ŕ��ˁB
			G_Player_MissileManager().missileShot(m_position, m_forward, camera_forward);
			//���˂����̂Ńt���O�����ɖ߂��B
			M_Lockonflag = false;
		}
	}
	G_Player_MissileManager().Update();
}

void Player::HPcontrol()
{
	//HP�؂�
	if (HP <= 0.0f) {
		HP = 0.0f;
		player_desflag = true;
	}
	m_gauge->HP_meter(HP, HP_MAX);
}

void Player::Energycontrol()
{
	//�G�i�W�[�؂�
	if (Energy <= 0.0f) {
		Energy = 0.0f;
		EnergyOutFlag = true;
	}

	//�������W�����v�����Ă��Ȃ��Ƃ��̓G�i�W�[���񕜂���B
	if ((MoveFlag == false) && (JumpFlag == false)){
		if (Energy < Energy_MAX) {
			//�ڒn���ɃG�i�W�[��
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
	//��s�������i�[����B
	static bool flyFlag = false;

	//A�{�^���������ꂽ��
	if ((g_pad[0].IsPress(enButtonA))&&(EnergyOutFlag==false)) {
		//�W�����v����B
		if (m_moveSpeed.y < 0.0f) {
			m_moveSpeed.y += 550.0f;
		}

		//LB1�{�^���������ĉ������Ă���Ƃ�
		if ((g_pad[0].IsPress(enButtonLB1)) && (EnergyOutFlag == false))
		{
			m_moveSpeed.y += 250.0f;
		}
		else {
			m_moveSpeed.y += 200.0f;	//������ɑ��x��ݒ肵�āA
		}
		
		if (m_charaCon.IsOnGround()) {
			m_moveSpeed.y += 850.0f;
		}
		m_charaCon.Jump();		//�L�����N�^�[�R���g���[���[�ɃW�����v�������Ƃ�ʒm����B
		Energy -= 5.0f;  //�G�i�W�[����
		JumpFlag = true;  //�W�����v�{�^���������ꂽ
		m_Jumpse.Play(true);
	}
	else {
		//�d��
		m_moveSpeed.y -= 400.0f;
		JumpFlag = false;
		m_Jumpse.Stop();
	}
	if (!m_charaCon.IsOnGround()) {
		flyFlag = true;
	}

	if (flyFlag==true) {
		//��s���ɒ��n������
		if (m_charaCon.IsOnGround()) {
			//���ʉ�(���n��)��炷�B
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
	//�V���h�E�L���X�^�[��o�^�B
	Shadow_map().RegistShadowCaster(&m_model);
}