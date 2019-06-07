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

struct Player_MineState {
	bool flag;//�}�C�����ݒu�����ǂ���
	bool explosion; //�����������ǂ���
	CVector3 m_position;
	SkinModel m_model;				  //�X�L�����f���B
	float blastrange;                 //�����͈́B
	bool desflag;                     //�������Ă��邩�H
};

struct Player_Blackhole {
	bool flag;//�e�ۂ����˒����ǂ���
	bool explosion; //�����������ǂ���
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //�X�L�����f���B
	BulletController m_bulletCon;    //�e�ۂ̍��́B
	int lengthcount;                   //�e�ۂ̈ړ������B
	bool desflag;                      //�������Ă��邩�H
	float time;
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
	///�}�C���̔��ˏ����B
	/// </summary>
	/// <remarks>
	/// Player���}�C����ݒu���悤�Ƃ���ƌĂяo����܂��B
	/// �}�C���̔z�񂩂�flag��false�̗v�f�������A
	/// ���̗v�f�ɂ��ꂩ��ݒu����}�C���̏����i�[����flag��true�ɂ��܂��B
	/// </remarks>
	/// <param name="p_position">�v���C���[�̍��W</param>
	void mineShot(const CVector3 p_position);

	void mineexplosion();

	/// <summary>
	///�u���b�N�z�[���e�̔��ˏ����B
	/// </summary>
	/// <remarks>
	/// Player���u���b�N�z�[���e�𔭎˂��悤�Ƃ���ƌĂяo����܂��B
	/// �e�ۂ̔z�񂩂�flag��false�̗v�f�������A
	/// ���̗v�f�ɂ��ꂩ�甭�˂���e�ۂ̏����i�[����flag��true�ɂ��܂��B
	/// </remarks>
	/// <param name="p_position">�v���C���[�̍��W</param>
	/// <param name="lightDir">�v���C���[�̑O��</param>
	void BlackholeShot(const CVector3 p_position, const CVector3 p_forward);

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
	*@brief	�u���b�N�z�[���e�̐���B
	*/
	void Blackhole_move(int bulletNumber);
	/*!
	*@brief	�G�Ƃ̓����蔻��B
	*/
	void EnemyHit(Enemy* enemy);

	/*!
	*@brief	�G�Ƃ̓����蔻��(�}�C��)�B
	*/
	void EnemyHitMine(Enemy* enemy);

	/*!
	*@brief	�G�Ƃ̓����蔻��(�u���b�N�z�[��)�B
	*/
	void EnemyHitBlackhole(Enemy* enemy);

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
	static const int Player_Mine_NUM = 5;          //�}�C���̔z��̐��B
	static const int Player_Blackhole_NUM = 1;     //�u���b�N�z�[���̔z��̐��B
	static const int Player_Missile_NUM = 10;      //�~�T�C���̔z��̐��B
	const float bulletmoveSpeed = 10000.0f;           //�e���B
	const float blackholeAbsorb = 2000.0f;         //�u���b�N�z�[���̋z���͈́B
	const float blackholeTime = 300.0f;         //�u���b�N�z�[���̐������ԁB
	const int BulletDamage = 10;
	const int MineDamage = 100;
	const int BlackholeDamage = 5;
	int count = 0;
	int minecount = 0;
	int blackholecount = 0;
	int missile_ammo_NUM;//�~�T�C���̒e��̐��B
	Player_BulletState bullet[Player_Bullet_NUM];  //�e�ۂ̔z��B    
	Player_MineState mine[Player_Mine_NUM];  //�}�C���̔z��B
	Player_Blackhole blackhole[Player_Blackhole_NUM];  //�u���b�N�z�[���̔z��B
	Player_MissileState missile[Player_Missile_NUM];//�~�T�C���̔z��B
	SkinModel m_modelproto;						   //�X�L�����f���̃v���g�^�C�v�B

	Effekseer::Effect* m_sampleEffect = nullptr;
	Effekseer::Effect* m_blackholeEffect = nullptr;
	Effekseer::Handle m_mineEffectHandle = -1;
	//Effekseer::Handle m_pBulletEffectHandle = -1;
};

//Player_BulletManager�̃C���X�^���X���擾�B
static inline Player_BulletManager& G_Player_BulletManager()
{
	return Player_BulletManager::Instance();
}
