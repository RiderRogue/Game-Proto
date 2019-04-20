#pragma once

#include "Player.h"
class Gamecamera : public IGameObject
{
public:
	Gamecamera();
	~Gamecamera();
	bool Start();
	void Update();

	/*!
	*@brief �v���C���[���W���擾����B
	*@param[in] Ppos    Player_positio�ɐݒ肷��v���C���[�̍��W�B
	*/
	void GetPlayer_position(CVector3 Ppos)
	{
		Player_position = Ppos;
	}

	/*!
	*@brief �Q�[���J�������W��ݒ肷��B
	*@param[in] Cpos    �J�����ɐݒ肷����W�B
	*/
	void SetCamera_position(CVector3 Cpos)
	{
		Camera_vector = Cpos;
	}

	/*!
	*@brief �v���C���[�̑O���������P�ʃx�N�g������A�v���C���[�̌������̒P�ʃx�N�g�����擾����B
	*/
	CVector3 SetPlayer_Cameravector()
	{
		CVector3 P_CVector;
		P_CVector = player->Getforward();
		P_CVector *= -1;
		return P_CVector;
	}

	/*!
	*@brief �Q�[���J�����̑O���̕����x�N�g����Ԃ��B
	*@return �Q�[���J�����̑O���̕����x�N�g���B
	*/
	CVector3 Getforward()
	{
		return m_forward;
	}

	/*!
	*@brief �Q�[���J�����̉E�����̕����x�N�g����Ԃ��B
	*@return �Q�[���J�����̉E�����̕����x�N�g���B
	*/
	CVector3 Getrite()
	{
		return m_rite;
	}
	/*!
	*@brief ��]�̎��Ƀv���C���[�̍��W��ݒ肷��B
	*/
	void qBiasSet_Playerposition()
	{
		CVector3 setposition = player->Getposition();
		qBias.x = setposition.x;
		qBias.y = setposition.y;
		qBias.z = setposition.z;
	}
	/*!
	*@brief �Q�[���J�����̏������B
	*/
	void Init();
private:
	const float Camera_Length_MAX = 200.0f;
	const float Camera_Length_MIN = 200.0f;
	float Camera_Length = Camera_Length_MAX;
	float Player_moveside = 0.0f;
	float rStick_x;                                     //�E�X�e�B�b�N�̉��̓��͗ʁB
	float rStick_y;                                     //�E�X�e�B�b�N�̏c�̓��͗ʁB
	float lStick_x;                                     //���X�e�B�b�N�̉��̓��͗ʁB
	float lStick_y;                                     //���X�e�B�b�N�̏c�̓��͗ʁB
	CVector3 Player_position = CVector3::Zero();        //�v���C���[�̍��W���i�[����B
	CVector3 Camera_vector = CVector3::Zero();          //�J�����̍��W�����B
	Player* player;                                     //�v���C���[�̍��W���擾���邽�߂Ɏg���B
	CMatrix mRot;                                       //�J�����̉�]�s��B
	CVector3 m_forward = CVector3::Zero();				//�J�����̑O���B
	CVector3 m_rite = CVector3::Zero();                 //�J�����̉E�����B
	CVector3 Player_Cameravector = CVector3::Zero();    //�v���C���[����J�����ւ̒P�ʃx�N�g��(�����x�N�g��)�B
	CQuaternion m_rotation = CQuaternion::Identity();   //�J�����̔C�ӎ���]�B
	CQuaternion qBias = CQuaternion::Identity();
	float rotation_speed;                               //�J�����̉�]�X�s�[�h�B
	bool boostflag = false;                             //�u�[�X�g���̃t���O�B
};