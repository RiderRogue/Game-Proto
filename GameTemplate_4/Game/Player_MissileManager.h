#pragma once
#include"Missile.h"
using namespace YTEngine;
class Player_MissileManager
{
public:
	Player_MissileManager();
	~Player_MissileManager();
	void Start();
	void Update();
	//void Draw();
	//void OnDestroy();
	/// <summary>
	///�~�T�C���̃��b�N�I�������B
	/// </summary>
	/// <remarks>
	/// Player���~�T�C���𔭎˂��悤�Ƃ���ƌĂяo����܂��B
	/// �~�T�C���̔z�񂩂�flag��false�̗v�f�������A
	/// ���̗v�f�ɂ��ꂩ�甭�˂���~�T�C���̏����i�[����flag��true�ɂ��܂��B
	/// </remarks>
	/// <param name="p_position">�v���C���[�̍��W</param>
	/// <param name="p_forward">�v���C���[�̑O��</param>
	/// <param name="c_forward">�J�����̑O��</param>
	void missileLock(const CVector3 p_position, const CVector3 p_forward, const CVector3 c_forward);

	/// <summary>
	///�~�T�C���̔��ˏ����B
	/// </summary>
	/// <remarks>
	/// Player���~�T�C���𔭎˂��悤�Ƃ���ƌĂяo����܂��B
	/// �~�T�C���̔z�񂩂�flag��false�̗v�f�������A
	/// ���̗v�f�ɂ��ꂩ�甭�˂���~�T�C���̏����i�[����flag��true�ɂ��܂��B
	/// </remarks>
	/// <param name="p_position">�v���C���[�̍��W</param>
	/// <param name="p_forward">�v���C���[�̑O��</param>
	/// <param name="c_forward">�J�����̑O��</param>
	void missileShot(const CVector3 p_position, const CVector3 p_forward, const CVector3 c_forward);

	/*!
	*@brief	�G�Ƃ̓����蔻��B
	*/
	bool EnemyHit(CVector3 m_Eposition);

	/*!
	*@brief	�C���X�^���X�̎擾�B
	*/
	static Player_MissileManager& Instance()
	{
		static Player_MissileManager instance;//Player_MissileManager�̃C���X�^���X�𐶐��B
		return instance;
	}

private:
	static const int Player_Missile_NUM = 10;       //�~�T�C���̔z��̐��B 
	//Missile missile[Player_Missile_NUM];//�~�T�C���̔z��B
	int missile_ammo_NUM;//�~�T�C���̒e��̐��B
	static const int Lockon_Enemy_NUM = 3;  //��x�Ƀ��b�N�I���ł���G�̐��B
};

//Player_MissileManager�̃C���X�^���X���擾�B
static inline Player_MissileManager& G_Player_MissileManager()
{
	return Player_MissileManager::Instance();
}


