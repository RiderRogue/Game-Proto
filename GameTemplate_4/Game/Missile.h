#pragma once
/*!
* @brief	�~�T�C���B
*/
//#include "Enemy.h"
#include "character/BulletController.h"
using namespace YTEngine;
class Enemy;
class Missile : public IGameObject
{
public:
	Missile();
	~Missile();
	bool Start();
	void Update();
	void Draw();
private:
	bool flag;//�e�ۂ����˒����ǂ���
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //�X�L�����f���B
	BulletController m_bulletCon;    //�e�ۂ̍��́B
	int lengthcount;                   //�e�ۂ̈ړ������B
	bool desflag;                      //�������Ă��邩�H
	Enemy* lockon_enemy;          //���b�N�I�������G�B
};

