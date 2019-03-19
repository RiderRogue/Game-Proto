#pragma once
/*!
* @brief	�G�l�~�[(�G)�B
*/
#include "Player.h"
#include "character/CharacterController.h"
using namespace YTEngine;
class Enemy : public IGameObject
{
public:
	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void Draw();
	//�V���h�E�L���X�^�[�ւ̓o�^�B
	void SetShadowCasters();
	/*!
	* @brief	�_���[�W�����S�t���O�B
	*@param[in]	damage		�󂯂��_���[�W�ʁB
	*/
	void enemyDamage(int damage);
	/*!
	*@brief	���S�t���O�̎擾�B
	*/
	bool Getenemy_deathflag()
	{
		return enemy_deathflag;
	}
	/*!
	*@brief	���W�̐ݒ�B
	*/
	void SetPosition(CVector3 position)
	{
		m_position = position;
		m_charaCon.SetPosition(position);
		m_position_center = position;
		m_position_center.y += (enemy_height / 2);
	}
	/*!
	*@brief	�����蔻��Ȃǂň����A���S���W�̎擾�B
	*/
	CVector3 GetPosition_center()
	{
		return m_position_center;
	}
	/*!
	*@brief	EnemyManager�ł̔z��ԍ��̕ۑ��B
	*/
	void SetslotNumber(int slotNumber)
	{
		My_slotNumber = slotNumber;
	}
private:
	CMatrix mRot;                                       //�G�̉�]�s��B
	CVector3 m_moveSpeed = CVector3::Zero();            //�G�̈ړ����x�B
	CVector3 m_forward = CVector3::Zero();				//�G�̑O���B
	CVector3 m_rite = CVector3::Zero();                 //�G�̉E���B
	CQuaternion m_rotation = CQuaternion::Identity();   //�L�����̉�]
	CQuaternion qBias = CQuaternion::Identity();
	CVector3 m_position = CVector3::Zero();             //�v���C���[�̍��W�B
	CVector3 m_position_center;
	SkinModel m_model;									//�X�L�����f���B
	CharacterController m_charaCon;                     //�G�̍��́B
	int enemy_HP;                                       //�G��HP�B
	const float enemy_height = 100.0f;		                //�G�̍����B
	const float enemy_weight = 30.0f;                         //�G�̔��a�B
	bool enemy_deathflag = false;                       //�G�̎��S�t���O�BManager�Ɏg���B
	Player* player;                                     //�v���C���[�̃|�C���^�B
	int My_slotNumber;              //EnemyManager��Ŏ��g�����݂��Ă���X���b�g�ԍ��B
};

