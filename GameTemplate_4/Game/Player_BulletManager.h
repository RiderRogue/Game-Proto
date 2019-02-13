#pragma once

#include "character/BulletController.h"
struct Player_BulletState {
	bool flag;//�e�ۂ����˒����ǂ���
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //�X�L�����f���B
	BulletController m_charaCon;    //�e�ۂ̍��́B
	int lengthcount;                   //�e�ۂ̈ړ������B
	bool desflag;                      //�������Ă��邩�H
};

class Player_BulletManager : public IGameObject
{
public:
	Player_BulletManager();
	~Player_BulletManager();
	bool Start();
	void Update();
	void Draw();
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
	void Shot(const CVector3 p_position, const CVector3 p_forward);
private:
	static const int Player_Bullet_NUM = 10;       //�e�ۂ̔z��̐��B 
	Player_BulletState bullet[Player_Bullet_NUM];  //�e�ۂ̔z��B    
	SkinModel m_modelproto;						   //�X�L�����f���̃v���g�^�C�v�B
	const float bulletmoveSpeed = 40.0f;           //�e���B
	int count = 0;
};


