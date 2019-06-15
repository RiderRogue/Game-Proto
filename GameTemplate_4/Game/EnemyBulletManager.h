#pragma once
#include"Player.h"
#include "character/BulletController.h"
using namespace YTEngine;
class Enemy_BulletState {
public:
	Enemy_BulletState(){}
	virtual ~Enemy_BulletState(){}
	virtual void Init(CVector3 eposition,CVector3 eforward);
	virtual void bulletmove(CVector3 P_pos);
	virtual void Draw();

	virtual void Setposition(CVector3 eposition) {
		m_position = eposition;
	}
	virtual CVector3 Getposition() {
		return m_position;
	}

	virtual void Setforward(CVector3 eforward) {
		m_forward = eforward;
	}
	virtual CVector3 Getforward() {
		return m_forward;
	}
	virtual int Getlengthcount() {
		return lengthcount;
	}
	virtual bool Gethitflag() {
		return hitflag;
	}
	virtual bool Getdesflag() {
		return desflag;
	}
protected:
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //�X�L�����f���B
	BulletController m_bulletCon;    //�e�ۂ̍��́B
	int lengthcount;                   //�e�ۂ̈ړ������B
	bool desflag;                      //�������Ă��邩�H
	bool hitflag;                     //�v���C���[�Ƀq�b�g�������H
	int lengthcountMAX;  //�G�e�̈ړ������̌��E�B
	float bulletmoveSpeed;           //�e���B
};

class Enemy_MissileState : public Enemy_BulletState
{
public:
	Enemy_MissileState() {}
	~Enemy_MissileState() {}
	void Init(CVector3 eposition, CVector3 eforward);
	void bulletmove(CVector3 P_pos);
private:
	CVector3 lockonPos;//���b�N�I�������v���C���[�̍��W�B
	CVector3 m_rite;
	CQuaternion m_rotation = CQuaternion::Identity();   //�~�T�C���̉�]
	CMatrix mRot;                                       //�G�̉�]�s��B
};

class EnemyBulletManager
{
public:
	EnemyBulletManager();
	~EnemyBulletManager();
	void Start();
	void Update();
	void Draw();
	void OnDestroy();
	/// <summary>
	///�e�ۂ̔��ˏ����B
	/// </summary>
	/// <remarks>
	/// Enemy���e�ۂ𔭎˂��悤�Ƃ���ƌĂяo����܂��B
	/// �e�ۂ̔z�񂩂�flag��false�̗v�f�������A
	/// ���̗v�f�ɂ��ꂩ�甭�˂���e�ۂ̏����i�[����flag��true�ɂ��܂��B
	/// </remarks>
	/// <param name="p_position">�G�̍��W</param>
	/// <param name="lightDir">�G�̑O��</param>
	void bulletShot(const CVector3 e_position, const CVector3 e_forward);

	/// <summary>
	///�~�T�C���̔��ˏ����B
	/// </summary>
	/// <remarks>
	/// Enemy���~�T�C���𔭎˂��悤�Ƃ���ƌĂяo����܂��B
	/// �~�T�C���̔z�񂩂�flag��false�̗v�f�������A
	/// ���̗v�f�ɂ��ꂩ�甭�˂���e�ۂ̏����i�[����flag��true�ɂ��܂��B
	/// </remarks>
	/// <param name="p_position">�G�̍��W</param>
	/// <param name="lightDir">�G�̑O��</param>
	void MissileShot(const CVector3 e_position, const CVector3 e_forward);

	//�s�v�ȓG�e�̍폜�B
	void erasebullet();

	//�s�v�ȓG�e�̑S�폜�B
	void erasebullet_All();

	/*!
	*@brief	�C���X�^���X�̎擾�B
	*/
	static EnemyBulletManager& Instance()
	{
		static EnemyBulletManager instance;//EnemyBulletManager�̃C���X�^���X�𐶐��B
		return instance;
	}
private:
	//�G�e���i�[����B
	std::list<Enemy_BulletState*>E_BulletList;
	
	const int lengthcountMAX = 50;  //�G�e�̈ړ������̌��E�B
	Player* player;
	float damage;           //�G�e�̗^����_���[�W�B
};
//EnemyBulletManager�̃C���X�^���X���擾�B
static inline EnemyBulletManager& G_EnemyBulletManager()
{
	return EnemyBulletManager::Instance();
}

