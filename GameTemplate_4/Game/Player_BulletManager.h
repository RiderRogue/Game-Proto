#pragma once

#include "character/BulletController.h"
#include "Enemy.h"
using namespace YTEngine;
struct Player_BulletState {
	bool flag;//�e�ۂ����˒����ǂ���
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //�X�L�����f���B
	BulletController m_bulletCon;    //�e�ۂ̍��́B
	int lengthcount;                   //�e�ۂ̈ړ������B
	bool desflag;                      //�������Ă��邩�H
};

struct Player_MissileState {
	bool flag;//�e�ۂ����˒����ǂ���
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //�X�L�����f���B
	BulletController m_bulletCon;    //�e�ۂ̍��́B
	int lengthcount;                   //�e�ۂ̈ړ������B
	bool desflag;                      //�������Ă��邩�H
	int lockon_enemyNumber;          //���b�N�I�������G�̃X���b�g�ԍ��B
};
class Player_BulletManager
{
public:
	Player_BulletManager();
	~Player_BulletManager();
	void Start();
	void Update();
	void Draw();
	void OnDestroy();
	/// <summary>
	///�e�ۂ̔��ˏ����B
	/// </summary>
	/// <remarks>
	/// Player���e�ۂ𔭎˂��悤�Ƃ���ƌĂяo����܂��B
	/// �e�ۂ̔z�񂩂�flag��false�̗v�f�������A
	/// ���̗v�f�ɂ��ꂩ�甭�˂���e�ۂ̏����i�[����flag��true�ɂ��܂��B
	/// </remarks>
	/// <param name="p_position">�v���C���[�̍��W</param>
	/// <param name="lightDir">�v���C���[�̑O��</param>
	void bulletShot(const CVector3 p_position, const CVector3 p_forward);

	/// <summary>
	///�~�T�C���̔��ˏ����B
	/// </summary>
	/// <remarks>
	/// Player���~�T�C���𔭎˂��悤�Ƃ���ƌĂяo����܂��B
	/// �~�T�C���̔z�񂩂�flag��false�̗v�f�������A
	/// ���̗v�f�ɂ��ꂩ�甭�˂���~�T�C���̏����i�[����flag��true�ɂ��܂��B
	/// </remarks>
	/// <param name="p_position">�v���C���[�̍��W</param>
	/// <param name="lightDir">�v���C���[�̑O��</param>
	void missileShot(const CVector3 p_position, const CVector3 p_forward);
	/*!
	*@brief	�ʏ�e�̐���B
	*/
	void Normalbullet_move(int bulletNumber);
	/*!
	*@brief	�~�T�C���̐���B
	*/
	void missile_move(int missiletNumber);

	/*!
	*@brief	�G�Ƃ̓����蔻��B
	*/
	void EnemyHit(Enemy* enemy);

	/*!
	*@brief	�C���X�^���X�̎擾�B
	*/
	static Player_BulletManager& Instance()
	{
		static Player_BulletManager instance;//Player_BulletManager�̃C���X�^���X�𐶐��B
		return instance;
	}
private:
	static const int Player_Bullet_NUM = 10;       //�e�ۂ̔z��̐��B 
	static const int Player_Missile_NUM = 10;       //�~�T�C���̔z��̐��B 
	Player_BulletState bullet[Player_Bullet_NUM];  //�e�ۂ̔z��B    
	Player_MissileState missile[Player_Missile_NUM];//�~�T�C���̔z��B
	SkinModel m_modelproto;						   //�X�L�����f���̃v���g�^�C�v�B
	const float bulletmoveSpeed = 10000.0f;           //�e���B
	int count = 0;
	int missile_ammo_NUM;//�~�T�C���̒e��̐��B
	const int BulletDamage = 10;
};

//Player_BulletManager�̃C���X�^���X���擾�B
static inline Player_BulletManager& G_Player_BulletManager()
{
	return Player_BulletManager::Instance();
}
