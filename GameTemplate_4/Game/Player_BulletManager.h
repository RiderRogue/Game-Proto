#pragma once

#include "character/BulletController.h"
#include "Enemy.h"
using namespace YTEngine;
class Player_BulletState {
public:
	Player_BulletState(){}
	virtual ~Player_BulletState(){}
	virtual void Init(CVector3 p_position, CVector3 p_forward);
	virtual void bulletmove();
	virtual void Draw();

	virtual void Hitbullet(Enemy* enemy);
	virtual bool Getdesflag() {
		return desflag;
	}
	//�G�e�Ƃ̓����蔻��B
	virtual bool HitE_bullet(CVector3 b_pos);
private:
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //�X�L�����f���B
	BulletController m_bulletCon;    //�e�ۂ̍��́B
	int lengthcount;                   //�e�ۂ̈ړ������B
	int lengthcount_MAX;                   //�ړ������̌��E�B
	int damage;                       //�_���[�W�B
	bool desflag;                      //�������Ă��邩�H
	float bulletmoveSpeed;   //�e�̈ړ����x�B
};

class Player_MineState {
public:
	Player_MineState(){}
	~Player_MineState(){}
	virtual void Init(CVector3 p_position);
	virtual void Draw();
	virtual void Hitmine(Enemy* enemy);
	virtual bool Getdesflag() {
		return desflag;
	}
	void Setexplosion(bool ex)
	{
		explosion = ex;
	}
	void Setdesflag(bool des)
	{
		desflag = des;
	}
private:
	bool explosion; //�����������ǂ���
	CVector3 m_position;
	SkinModel m_model;				  //�X�L�����f���B
	float blastrange;                 //�����͈́B
	bool desflag;                     //�������Ă��邩�H
	int MineDamage;
};

class Player_Blackhole {
public:
	Player_Blackhole() {}
	virtual ~Player_Blackhole(){}
	virtual void Init(CVector3 p_position, CVector3 p_forward);
	virtual void bulletmove(Effekseer::Effect* m_blackholeEffect);
	virtual void Draw();

	virtual void HitBlackhole(Enemy* enemy, Effekseer::Effect* m_blackholeEffect);
	virtual bool Getdesflag() {
		return desflag;
	}
	//�G�e�Ƃ̓����蔻��B
	virtual bool HitE_bullet(CVector3 b_pos);

	CVector3 AbsorbEnemyBullet(CVector3 e_bullet);
private:
	
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //�X�L�����f���B
	BulletController m_bulletCon;    //�e�ۂ̍��́B
	int lengthcount;                   //�e�ۂ̈ړ������B
	int lengthcount_MAX;              //�ړ������̌��E�B
	int BlackholeDamage;//���t���[�����Ƃ̃_���[�W�B
	bool explosion; //�����������ǂ���
	bool desflag;                      //�������Ă��邩�H
	float time;
	float bulletmoveSpeed;   //�e�̈ړ����x�B
	float blackholeTime;         //�u���b�N�z�[���̐������ԁB
	float blackholeAbsorb;         //�u���b�N�z�[���̋z���͈́B
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
	Player_BulletManager(){}
	~Player_BulletManager(){}
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

	void mineexplosion();//�S�N��

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
	*@brief	�~�T�C���̐���B
	*/
	void missile_move(int missiletNumber);

	
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
	
	/*!
	*@brief �u���b�N�z�[���̓G�e�z���B
	*@param[in] e_bullet    �J�����̉E�����̒P�ʃx�N�g���B
	*@return �z�����ވړ��x�N�g���B
	*/
	CVector3 Blackhole_EnemyBullet(CVector3 e_bullet);
	//�s�v�ȓG�e�̍폜�B
	void erasebullet();
	//�s�v�ȃv���C���[�̒e�̑S�폜�B
	void erasebullet_All();
	//�G�e�Ƃ̓����蔻��B
	bool E_Bullet_toHit(CVector3 b_pos);
private:
	static const int Player_Missile_NUM = 10;      //�~�T�C���̔z��̐��B	
	int count = 0;
	int minecount = 0;
	int blackholecount = 0;
	int missile_ammo_NUM;//�~�T�C���̒e��̐��B   
	Player_MissileState missile[Player_Missile_NUM];//�~�T�C���̔z��B

	Effekseer::Effect* m_sampleEffect = nullptr;
	Effekseer::Effect* m_blackholeEffect = nullptr;
	Effekseer::Handle m_mineEffectHandle = -1;

	//�v���C���[�̒e���i�[����B
	std::list<Player_BulletState*>P_BulletList;
	//�v���C���[�̃}�C�����i�[����B
	std::list<Player_MineState*>P_MineList; 
	//�v���C���[�̃}�C�����i�[����B
	std::list<Player_Blackhole*>P_BlackholeList;
	//Effekseer::Handle m_pBulletEffectHandle = -1;
};

//Player_BulletManager�̃C���X�^���X���擾�B
static inline Player_BulletManager& G_Player_BulletManager()
{
	return Player_BulletManager::Instance();
}
