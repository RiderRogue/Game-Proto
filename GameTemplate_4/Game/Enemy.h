#pragma once
/*!
* @brief	�G�l�~�[(�G)�B
*/
#include"Player.h"
#include "character/EnemyController.h"

using namespace YTEngine;
class Enemy : public IGameObject
{
public:
	Enemy();
	virtual~Enemy();
	virtual bool Start();
	virtual void Update();
	virtual void Draw();
	virtual void PostDraw();
	//�T�[�`��Ԃł̍X�V�B
	virtual void SearchUpdate();
	//�U����Ԃł̍X�V�B
	virtual void AttackUpdate();
	enum EnemyState
	{
		Enemy_Search,
		Enemy_Attack,
		Enemy_Num
	};
	//�v���C���[�̃T�[�`�B
	virtual void SearchPlayer();
	//�V���h�E�L���X�^�[�ւ̓o�^�B
	void SetShadowCasters();
	/*!
	* @brief	�_���[�W�����S�t���O�B
	*@param[in]	damage		�󂯂��_���[�W�ʁB
	*/
	virtual void enemyDamage(int damage);

	virtual void enemyDamage_Blackhole(int damage,CVector3 b_position);
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
	virtual void SetPosition(CVector3 position)
	{
		m_position = position;
		m_charaCon.SetPosition(position);
		m_position_center = position;
		m_position_center.y += (enemy_height / 2);
	}
	/*!
	*@brief	�����蔻��Ȃǂň����A���S���W�̎擾�B
	*/
	virtual CVector3 GetPosition_center()
	{
		return m_position_center;
	}
	/*!
	*@brief	EnemyManager�ł̔z��ԍ��̕ۑ��B
	*/
	virtual void SetslotNumber(int slotNumber)
	{
		My_slotNumber = slotNumber;
	}

	//�G�e�̔��ˏ����B
	virtual void EnemyShot();
	//�v���C���[�����։�]�B
	void PlayertoRotation();
	//2�̃x�N�g���̊p�x���p�x�\�L(degree)�ŕԂ��B
	float VectorAngleDeg(CVector3 c);
protected:
	CMatrix mRot;                                       //�G�̉�]�s��B
	CVector3 vDist;                                     //��Ɨp�B	
	CVector3 m_moveSpeed = CVector3::Zero();            //�G�̈ړ����x�B
	CVector3 m_premoveSpeed = CVector3::Zero();         //�N���X�O����̓G�̈ړ����x�B
	CVector3 m_forward = CVector3::Zero();				//�G�̑O���B
	CVector3 m_rite = CVector3::Zero();                 //�G�̉E���B
	CVector3 m_position = CVector3::Zero();             //�G�̍��W�B
	CVector3 m_position_center;                         //�����蔻��Ȃǂň������S���W�B
	CQuaternion m_rotation = CQuaternion::Identity();   //�L�����̉�]
	CQuaternion t_rotation = CQuaternion::Identity();   //�L�����̉�]
	CQuaternion qBias = CQuaternion::Identity();
	SkinModel m_model;									//�X�L�����f���B
	SkinModel t_model;//Targetsight�B
	EnemyController m_charaCon;                     //�G�̍��́B
	Player* player;                                     //�v���C���[�̃|�C���^�B
	EnemyState e_state;
	bool enemy_damageflag = false;                          //�G�̃_���[�W����B
	bool enemy_deathflag = false;                       //�G�̎��S�t���O�BManager�Ɏg���B
	int enemy_HP;                                       //�G��HP�B
	int My_slotNumber;              //EnemyManager��Ŏ��g�����݂��Ă���X���b�g�ԍ��B
	int count = 0;                     //�G�e�̔��ˊԊu�B
	int damagecount = 0;
	float enemy_height = 100.0f;		                //�G�̍����B
	float enemy_weight = 30.0f;                         //�G�̔��a�B
	float BulletRange;              //�˒��͈́B
	float Search_movespeed;         //�ʏ펞�̈ړ����x�B
};

