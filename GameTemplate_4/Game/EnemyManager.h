#pragma once
class Enemy;
using namespace YTEngine;

struct EnemyState {
	Enemy* enemy;
	bool Aliveflag;//�G�������Ă��邩�������t���O�B
};
class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();
	void Start();
	void Update();
	void EnemyBulletDraw();
	/*!
	*@brief	�G�̃X�|�[���B
	*/
	void spawnenemy(CVector3 position);
	/*!
	*@brief	EnemyManager�C���X�^���X�̎擾�B
	*/
	static EnemyManager& Instance()
	{
		static EnemyManager instance;//EnemyManager�̃C���X�^���X�𐶐��B
		return instance;
	}
	/*!
	*@brief	�G�̑S�폜�B�Q�[���I�����̏������Ɏg���B
	*/
	void AllDeleteEnemy();
private:
	/*!
	*@brief	�G�̎��S�����B
	*/
	void Deleteenemy();
	static const int Enemy_NUM = 10;       //�G�̔z��̐��B
	static const int LockonEnemy_NUM = 3;  //��x�Ƀ��b�N�I���ł���G�̐��B
	EnemyState enemy_slot[Enemy_NUM];
};

//EnemyManager�̃C���X�^���X���擾�B
static inline EnemyManager& G_EnemyManager()
{
	return EnemyManager::Instance();
}
