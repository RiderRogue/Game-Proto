#include "stdafx.h"
#include "Gamecamera.h"


Gamecamera::Gamecamera()
{
}


Gamecamera::~Gamecamera()
{
}

bool Gamecamera::Start()
{
	mRot.MakeRotationFromQuaternion(m_rotation);
	//�O��
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
	m_forward.Normalize();

	//�E����
	m_rite.x = mRot.m[0][0];
	m_rite.y = mRot.m[0][1];
	m_rite.z = mRot.m[0][2];
	m_rite.Normalize();

	player = FindGO<Player>("Player");
	//�v���C���[�̑O�����x�N�g������������x�N�g�����擾����B
	SetPlayer_Cameravector();
	Camera_vector = Player_Cameravector * 300.0f;//��������300�قǉ����������W�ɐݒ肷��B
	Camera_vector.y = 120.0f;
	g_camera3D.SetTarget(player->Getposition());
	g_camera3D.SetPosition(Camera_vector);
	g_camera3D.Update();
	


	//�Q�[���J�����̉�]�X�s�[�h���������B
	rotation_speed = 10.0f;
	return true;
}

void Gamecamera::Init()
{
	//�v���C���[�̑O�����x�N�g������������x�N�g�����擾����B
	SetPlayer_Cameravector();
	//��������300�قǉ����������W�ɐݒ肷��B
	Camera_vector = Player_Cameravector * 300.0f;
	Camera_vector.y += 120.0f;
}

void Gamecamera::Update()
{
	//�Q�[���J�����̉�]�X�s�[�h���v���C���[�ɑ���B
	player->Setcamera_rot_speed(rotation_speed);

	//�Q�[���J�����̒����_(�^�[�Q�b�g)��ݒ肷��B
	CVector3 Targetposition = player->Getposition();
	Targetposition.y += 120.0f;
	//r�X�e�B�b�N�̓��͗ʂ��󂯎��B
	rStick_x = g_pad[0].GetRStickXF();
	//r�X�e�B�b�N�̓��͗ʂ��v���C���[�ɑ���B
	player->Setcamera_rot_angle(rStick_x);

	qBias.SetRotationDeg(g_camera3D.GetUp(), rStick_x*rotation_speed);
	qBias.Multiply(Player_Cameravector);
	m_rotation.Multiply(qBias);

	mRot.MakeRotationFromQuaternion(m_rotation);
	//�O����
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
	m_forward.Normalize();
	//�E����
	m_rite.x = mRot.m[0][0];
	m_rite.y = mRot.m[0][1];
	m_rite.z = mRot.m[0][2];
	m_rite.Normalize();
	
	Camera_vector = Player_Cameravector * 300.0f;
	

	CVector3 Camera_position = player->Getposition() + Camera_vector;
	Camera_position.y += player->Getheight()+50.0f;
	//�v���C���[�ɃQ�[���J�����̑O�����ƉE�����̒P�ʃx�N�g���𑗂�B
	player->SetCamera_forward(m_forward);
	player->SetCamera_rite(m_rite);

	

	g_camera3D.SetTarget(Targetposition);
	g_camera3D.SetPosition(Camera_position);
	//g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	//g_camera3D.SetPosition({ 0.0f, 200.0f, 300.0f });
	g_camera3D.Update();
}