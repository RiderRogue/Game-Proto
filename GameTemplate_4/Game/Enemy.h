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
	*@brief	���S�t���O�̎擾�B
	*/
	bool Getenemy_deathflag()
	{
		return enemy_deathflag;
	}
private:
	CMatrix mRot;                                       //�G�̉�]�s��B
	CVector3 m_moveSpeed = CVector3::Zero();            //�G�̈ړ����x�B
	CVector3 m_forward = CVector3::Zero();				//�G�̑O���B
	CVector3 m_rite = CVector3::Zero();                 //�G�̉E���B
	CQuaternion m_rotation = CQuaternion::Identity();   //�L�����̉�]
	CQuaternion qBias = CQuaternion::Identity();
	CVector3 m_position = CVector3::Zero();             //�v���C���[�̍��W�B
	SkinModel m_model;									//�X�L�����f���B
	CharacterController m_charaCon;                     //�G�̍��́B
	float enemy_height = 100.0f;		                //�G�̍����B
	float enemy_weight = 30.0f;                         //�G�̔��a�B
	bool enemy_deathflag = false;                       //�G�̎��S�t���O�BManager�Ɏg���B
	Player* player;                                     //�v���C���[�̃|�C���^�B
};

