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
	SkinModel m_model;				   //スキンモデル。
	BulletController m_bulletCon;    //弾丸の剛体。
	int lengthcount;                   //弾丸の移動距離。
	bool desflag;                      //生存しているか？
	bool hitflag;                     //プレイヤーにヒットしたか？
	int lengthcountMAX;  //敵弾の移動距離の限界。
	float bulletmoveSpeed;           //弾速。
};

class Enemy_MissileState : public Enemy_BulletState
{
public:
	Enemy_MissileState() {}
	~Enemy_MissileState() {}
	void Init(CVector3 eposition, CVector3 eforward);
	void bulletmove(CVector3 P_pos);
private:
	CVector3 lockonPos;//ロックオンしたプレイヤーの座標。
	CVector3 m_rite;
	CQuaternion m_rotation = CQuaternion::Identity();   //ミサイルの回転
	CMatrix mRot;                                       //敵の回転行列。
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
	///弾丸の発射処理。
	/// </summary>
	/// <remarks>
	/// Enemyが弾丸を発射しようとすると呼び出されます。
	/// 弾丸の配列からflagがfalseの要素を見つけ、
	/// その要素にこれから発射する弾丸の情報を格納してflagをtrueにします。
	/// </remarks>
	/// <param name="p_position">敵の座標</param>
	/// <param name="lightDir">敵の前方</param>
	void bulletShot(const CVector3 e_position, const CVector3 e_forward);

	/// <summary>
	///ミサイルの発射処理。
	/// </summary>
	/// <remarks>
	/// Enemyがミサイルを発射しようとすると呼び出されます。
	/// ミサイルの配列からflagがfalseの要素を見つけ、
	/// その要素にこれから発射する弾丸の情報を格納してflagをtrueにします。
	/// </remarks>
	/// <param name="p_position">敵の座標</param>
	/// <param name="lightDir">敵の前方</param>
	void MissileShot(const CVector3 e_position, const CVector3 e_forward);

	//不要な敵弾の削除。
	void erasebullet();

	//不要な敵弾の全削除。
	void erasebullet_All();

	/*!
	*@brief	インスタンスの取得。
	*/
	static EnemyBulletManager& Instance()
	{
		static EnemyBulletManager instance;//EnemyBulletManagerのインスタンスを生成。
		return instance;
	}
private:
	//敵弾を格納する。
	std::list<Enemy_BulletState*>E_BulletList;
	
	const int lengthcountMAX = 50;  //敵弾の移動距離の限界。
	Player* player;
	float damage;           //敵弾の与えるダメージ。
};
//EnemyBulletManagerのインスタンスを取得。
static inline EnemyBulletManager& G_EnemyBulletManager()
{
	return EnemyBulletManager::Instance();
}

