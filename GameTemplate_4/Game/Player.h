#pragma once
/*!
* @brief	�v���C���[�B
*/
#include "character/CharacterController.h"
#include "Player_BulletManager.h"
#include "Player_MissileManager.h"
#include "gauge.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

using namespace YTEngine;
class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Draw();
	void PostDraw();
	void OnDestroy();
	//�V���h�E�L���X�^�[�ւ̓o�^�B
	void SetShadowCasters();

	/*!
	*@brief ���X�e�B�b�N��X���̓��͗ʂ�Ԃ��B
	*@return ���X�e�B�b�N��X���̓��͗ʁB
	*/
	float GetlStick_x()
	{
		return lStick_x;
	}

	/*!
	*@brief ���X�e�B�b�N��Y���̓��͗ʂ�Ԃ��B
	*@return ���X�e�B�b�N��Y���̓��͗ʁB
	*/
	float GetlStick_y()
	{
		return lStick_y;
	}
	/*!
	*@brief �v���C���[�̑O���̐��K���x�N�g����Ԃ��B
	*@return �v���C���[�̑O���x�N�g���B
	*/
	CVector3 Getforward()
	{
		return m_forward;
	}
	/*!
	*@brief �v���C���[�̉E���̐��K���x�N�g����Ԃ��B
	*@return �v���C���[�̉E���x�N�g���B
	*/
	CVector3 Getrite()
	{
		return m_rite;
	}
	/*!
	*@brief	�v���C���[�̍��W��Ԃ��B
	*@return �v���C���[�̍��W�B
	*/
	CVector3 Getposition()
	{
		return m_position;
	}
	/*!
	*@brief	�v���C���[�̈ړ����x��Ԃ��B
	*@return �v���C���[�̈ړ����x�B
	*/
	CVector3 GetmoveSpeed()
	{
		return m_moveSpeed;
	}
	/*!
	*@brief	�v���C���[�̉������̈ړ����x��Ԃ��B
	*@return �v���C���[�̉������̈ړ����x�B
	*/
	float GetmoveSpeed_side()
	{
		return m_moveSpeed_side;
	}
	/*!
	*@brief	�v���C���[���u�[�X�g���Ă��邩��Ԃ��B
	*@return �u�[�X�g���[�h���̃t���O�B
	*/
	bool Getboostflag()
	{
		return MoveFlag;
	}
	/*!
	*@brief �Q�[���J�����̑O���̒P�ʃx�N�g����ݒ肷��B
	*@param[in] c_forward    �J�����̑O���̒P�ʃx�N�g���B
	*/
	void SetCamera_forward(CVector3 c_forward)
	{
		camera_forward = c_forward;
	}

	/*!
	*@brief �Q�[���J�����̉E�����̒P�ʃx�N�g����ݒ肷��B
	*@param[in] c_rite    �J�����̉E�����̒P�ʃx�N�g���B
	*/
	void SetCamera_rite(CVector3 c_rite)
	{
		camera_rite = c_rite;
	}

	/*!
	*@brief �Q�[���J�����̉E�X�e�B�b�N�̓��͗ʂ�ݒ肷��B
	*@param[in] angle    �J�����̉E�X�e�B�b�N�̓��͗�(-1�`1�͈̔�)�B
	*/
	void Setcamera_rot_angle(float angle)
	{
		camera_rot_angle = angle;
	}

	/*!
	*@brief �Q�[���J�����̉�]�X�s�[�h��ݒ肷��B
	*@param[in] angle   �Q�[���J�����̉�]�X�s�[�h�B
	*/
	void Setcamera_rot_speed(float angle)
	{
		camera_rot_speed = angle;
	}
	/*!
	*@brief �ˊp��ݒ肷��B
	*@param[in] angle   �Q�[���J�����̑O���̒P�ʃx�N�g���B
	*/
	void Setbullet_angle(float angle)
	{
		bullet_angle = angle;
	}
	/*!
	*@brief	�v���C���[�̍U�����͂������B
	*/
	void Bullet_Missile_Controller();
	/*!
	*@brief	�v���C���[�̃W�����v�֘A�̏����B
	*/
	void Player_Jump();
	/*
	*@brief �v���C���[�̍�����Ԃ��B
	*@param[in]	height	�v���C���[�̍����B
	*/
	float Getheight()
	{
		return player_height;
	}
	/*
	*@brief �v���C���[�̈ړ����̏����B
	*/
	void MoveEffect();

	/*
	*@brief �v���C���[�̃G�i�W�[�̊Ǘ��B
	*/
	void Energycontrol();
private:
	

	CMatrix mRot;                                       //�v���C���[�̉�]�s��B
	CVector3 m_moveSpeed = CVector3::Zero();            //�v���C���[�̈ړ����x�B
	CVector3 m_forward = CVector3::Zero();				//�v���C���[�̑O���B
	CVector3 m_rite = CVector3::Zero();                 //�v���C���[�̉E���B
	CVector3 m_position = CVector3::Zero();             //�v���C���[�̍��W�B
	CVector3 camera_forward = CVector3::Zero();			//�Q�[���J�����̑O���B
	CVector3 camera_rite = CVector3::Zero();            //�Q�[���J�����̉E�����B
	CVector3 Bullet_vector = CVector3::Zero();            //�ˌ������B
	CVector3 m_targetSight_position = { 0.0f,130.0f,0.0f };//�^�[�Q�b�g�T�C�g�B
	SkinModel m_model;									//�X�L�����f���B
	CharacterController m_charaCon;                     //�v���C���[�̍��́B
	CQuaternion m_rotation = CQuaternion::Identity();   //�L�����̉�]
	CQuaternion qBias = CQuaternion::Identity();
	ID3D11ShaderResourceView* g_normalMapSRV = nullptr; //�@���}�b�v�B
	Player_BulletManager* bulletManager;
	Sprite targetSight;
	gauge* m_gauge;
	CSoundEngine m_soundEngine;				//�T�E���h�G���W���B
	CSoundSource m_bgm[2];					//BGM�B
	CSoundSource m_se[3];					//SE�B
	CSoundSource m_movese;
	CSoundSource m_Jumpse;

	float m_moveSpeed_side = 0.0f;          //�v���C���[�̉������̈ړ����x�B
	float lStick_x;                         //���X�e�B�b�N�̉��̓��͗ʁB
	float lStick_y;                         //���X�e�B�b�N�̏c�̓��͗ʁB
	float player_height = 100.0f;		    //�v���C���[�̍����B
	float camera_rot_angle = 0.0f;          //�Q�[���J������������������(-1�`1�͈̔�)�B
	float camera_rot_speed = 0.0f;          //�Q�[���J�����̉�]�X�s�[�h�B
	float Energy;
	float bullet_angle = 0.0f;              //�ˊp�B
	float boostTime = 0.0f;                 //�������Ă��鎞�ԁB
	const float Energy_MAX = 1000.0f;
	bool EnergyOutFlag = false;             //�G�i�W�[�؂�����m�B
	bool MoveFlag = false;                  //�������Ă��邩��ۑ��B      
	bool JumpFlag = false;        
	bool player_rotationFlag = true;        //�v���C���[����]���邩�𔻒肷��t���O�Btrue�ŉ�]�B	                           
};

