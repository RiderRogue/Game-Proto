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
	Player_Cameravector = SetPlayer_Cameravector();
	Camera_vector = Player_Cameravector * Camera_Length;//��������300�قǉ����������W�ɐݒ肷��B
	Camera_vector.y = 120.0f;
	//Camera_vector.x += 120.0f;
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
	Player_Cameravector = SetPlayer_Cameravector();
	//��������300�قǉ����������W�ɐݒ肷��B
	Camera_vector = Player_Cameravector * Camera_Length;
	Camera_vector.y += 120.0f;
}

void Gamecamera::Update()
{
	//�Q�[���J�����̉�]�X�s�[�h���v���C���[�ɑ���B
	player->Setcamera_rot_speed(rotation_speed);

	//�Q�[���J�����̒����_(�^�[�Q�b�g)��ݒ肷��B
	CVector3 Targetposition = player->Getposition();
	Targetposition.y += 100.0f;
	//Targetposition.x -= 120.0f;
	//R�X�e�B�b�N�̓��͗ʂ��󂯎��B
	rStick_x = g_pad[0].GetRStickXF();
	rStick_y = g_pad[0].GetRStickYF();
	//L�X�e�B�b�N�̓��͗ʂ��󂯎��B
	lStick_x = g_pad[0].GetLStickXF();
	lStick_y = g_pad[0].GetLStickYF();
	//R�X�e�B�b�N�̓��͗ʂ��v���C���[�ɑ���B
	player->Setcamera_rot_angle(rStick_x);
	boostflag = player->Getboostflag();

	//L�X�e�B�b�N�̓��͗ʂ̐�Βl���󂯎��B
	float C_minasuLength = lStick_x;
	if (C_minasuLength < 0) {
		C_minasuLength *= -1;
	}
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

	//�o�b�N�������ɃJ�������߂Â���B
	if (lStick_y<0.0f) {
		if (Camera_Length> Camera_Length_MIN) {
			Camera_Length -= 5.0f;
		}
	}
	else {
		if (Camera_Length < Camera_Length_MAX) {
			Camera_Length += 5.0f;
		}
	}
	//Camera_Length = Camera_Length_MAX - (C_minasuLength*150.0f);
	

	Camera_vector = Player_Cameravector * Camera_Length;
	
	

	CVector3 Camera_position = player->Getposition() + Camera_vector;
	Camera_position.y += player->Getheight()+10.0f;
	//�v���C���[�ɃQ�[���J�����̑O�����ƉE�����̒P�ʃx�N�g���𑗂�B
	player->SetCamera_forward(m_forward);
	player->SetCamera_rite(m_rite);

	//�O�t���[���ł̂��ꂼ��̒l��ۑ�����B
	static CVector3 Post_Camera_position = CVector3::Zero();
	static CVector3 Post_Targetposition = CVector3::Zero();
	Player_moveside = player->GetmoveSpeed_side();

	static float cameraspeed = 0.0f;
	static const float cameraspeedMIN = -60.0f;
	static const float cameraspeedMIN_boost = -150.0f;
	if (Player_moveside == 0.0f) {
		cameraspeed = 0.0f;
		/*if (cameraspeed<0.0f) {
			cameraspeed += 10.0f;
		}
		else {
			cameraspeed = 0.0f;
		}*/
		/*if () {
		Camera_position = Post_Camera_position *m_rite * lStick_x*(-50.0f);
		Targetposition -= m_rite * lStick_x*(-50.0f);
	}*/
	}
	else {		
		//2�̒l�̉��̈ړ��ʂ͈ꏏ�B
		//�u�[�X�g���ɂ̓J�����̑��x���グ��B
		if (boostflag == true) {
			if (cameraspeed > cameraspeedMIN_boost) {
				cameraspeed -= 10.0f;
			}
			else {
				cameraspeed = cameraspeedMIN_boost;
			}
		}
		else {
			/*�u�[�X�g������ɃJ�������x���u�[�X�g�����݂ɑ����Ƃ���
			 ��u�[�X�g���̑��x�ɖ߂��B*/
			if (cameraspeed < cameraspeedMIN) {
				cameraspeed += 10.0f;
			}
			else {
				//��u�[�X�g���̑��x�Ȃ�ʏ�̏����B
				if (cameraspeed > cameraspeedMIN) {
					cameraspeed -= 5.0f;
				}
				else {
					cameraspeed = cameraspeedMIN;
				}
			}
		}

			Camera_position += m_rite * lStick_x *(cameraspeed);
			Targetposition += m_rite * lStick_x*(cameraspeed);
			Post_Camera_position = Camera_position;
			Post_Targetposition = Targetposition;
		
	}

	//�J�����̏㉺����B
	if (rStick_y<0.0f) {
		Camera_position.y += rStick_y * (-100.0f);
	}
	else {
		Targetposition.y += rStick_y * (100.0f);
	}
	
	//�ˊp�̐ݒ�B
	CVector3 backvector = m_forward*(-1);
	CVector3 camera_bulletangle = Camera_position - Targetposition;
	camera_bulletangle.Normalize();
	player->Setbullet_angle(backvector.Dot(camera_bulletangle));
	/*if (g_pad[0].IsPress(enButtonRB2)) {
		Camera_position += m_rite * lStick_x*(100.0f);
		Targetposition += m_rite * lStick_x*(100.0f);
	}*/

	//Camera_position += m_rite*lStick_x * (-50.0f);

	g_camera3D.SetTarget(Targetposition);
	g_camera3D.SetPosition(Camera_position);
	//g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	//g_camera3D.SetPosition({ 0.0f, 200.0f, 300.0f });
	g_camera3D.Update();
}