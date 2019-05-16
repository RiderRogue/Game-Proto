#pragma once
#include"Player.h"
#include "character/BulletController.h"
using namespace YTEngine;
class Enemy_BulletState {
public:
	Enemy_BulletState(){}
	~Enemy_BulletState(){}
	void Init(CVector3 eposition,CVector3 eforward);
	void bulletmove(float bulletmoveSpeed, int lengthcountMAX);
	void Draw();

	void Setposition(CVector3 eposition) {
		m_position = eposition;
	}
	CVector3 Getposition() {
		return m_position;
	}

	void Setforward(CVector3 eforward) {
		m_forward = eforward;
	}
	CVector3 Getforward() {
		return m_forward;
	}
	int Getlengthcount() {
		return lengthcount;
	}
	bool Gethitflag() {
		return hitflag;
	}
	bool Getdesflag() {
		return desflag;
	}
private:
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //�X�L�����f���B
	BulletController m_bulletCon;    //�e�ۂ̍��́B
	int lengthcount;                   //�e�ۂ̈ړ������B
	bool desflag;                      //�������Ă��邩�H
	bool hitflag;                     //�v���C���[�Ƀq�b�g�������H
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
	const float bulletmoveSpeed = 2000.0f;           //�e���B
	const int lengthcountMAX = 50;  //�G�e�̈ړ������̌��E�B
	Player* player;
	float damage;           //�G�e�̗^����_���[�W�B
};
//EnemyBulletManager�̃C���X�^���X���擾�B
static inline EnemyBulletManager& G_EnemyBulletManager()
{
	return EnemyBulletManager::Instance();
}

