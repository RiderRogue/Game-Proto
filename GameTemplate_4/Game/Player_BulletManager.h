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
	//敵弾との当たり判定。
	virtual bool HitE_bullet(CVector3 b_pos);
private:
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //スキンモデル。
	BulletController m_bulletCon;    //弾丸の剛体。
	int lengthcount;                   //弾丸の移動距離。
	int lengthcount_MAX;                   //移動距離の限界。
	int damage;                       //ダメージ。
	bool desflag;                      //生存しているか？
	float bulletmoveSpeed;   //弾の移動速度。
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
	bool explosion; //爆発したかどうか
	CVector3 m_position;
	SkinModel m_model;				  //スキンモデル。
	float blastrange;                 //爆発範囲。
	bool desflag;                     //生存しているか？
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
	//敵弾との当たり判定。
	virtual bool HitE_bullet(CVector3 b_pos);

	CVector3 AbsorbEnemyBullet(CVector3 e_bullet);
private:
	
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //スキンモデル。
	BulletController m_bulletCon;    //弾丸の剛体。
	int lengthcount;                   //弾丸の移動距離。
	int lengthcount_MAX;              //移動距離の限界。
	int BlackholeDamage;//毎フレームごとのダメージ。
	bool explosion; //爆発したかどうか
	bool desflag;                      //生存しているか？
	float time;
	float bulletmoveSpeed;   //弾の移動速度。
	float blackholeTime;         //ブラックホールの生成時間。
	float blackholeAbsorb;         //ブラックホールの吸収範囲。
};
struct Player_MissileState {
	bool flag;//弾丸が発射中かどうか
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //スキンモデル。
	BulletController m_bulletCon;    //弾丸の剛体。
	int lengthcount;                   //弾丸の移動距離。
	bool desflag;                      //生存しているか？
	int lockon_enemyNumber;          //ロックオンした敵のスロット番号。
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
	///弾丸の発射処理。
	/// </summary>
	/// <remarks>
	/// Playerが弾丸を発射しようとすると呼び出されます。
	/// 弾丸の配列からflagがfalseの要素を見つけ、
	/// その要素にこれから発射する弾丸の情報を格納してflagをtrueにします。
	/// </remarks>
	/// <param name="p_position">プレイヤーの座標</param>
	/// <param name="lightDir">プレイヤーの前方</param>
	void bulletShot(const CVector3 p_position, const CVector3 p_forward);

	/// <summary>
	///マインの発射処理。
	/// </summary>
	/// <remarks>
	/// Playerがマインを設置しようとすると呼び出されます。
	/// マインの配列からflagがfalseの要素を見つけ、
	/// その要素にこれから設置するマインの情報を格納してflagをtrueにします。
	/// </remarks>
	/// <param name="p_position">プレイヤーの座標</param>
	void mineShot(const CVector3 p_position);

	void mineexplosion();//全起爆

	/// <summary>
	///ブラックホール弾の発射処理。
	/// </summary>
	/// <remarks>
	/// Playerがブラックホール弾を発射しようとすると呼び出されます。
	/// 弾丸の配列からflagがfalseの要素を見つけ、
	/// その要素にこれから発射する弾丸の情報を格納してflagをtrueにします。
	/// </remarks>
	/// <param name="p_position">プレイヤーの座標</param>
	/// <param name="lightDir">プレイヤーの前方</param>
	void BlackholeShot(const CVector3 p_position, const CVector3 p_forward);

	/// <summary>
	///ミサイルの発射処理。
	/// </summary>
	/// <remarks>
	/// Playerがミサイルを発射しようとすると呼び出されます。
	/// ミサイルの配列からflagがfalseの要素を見つけ、
	/// その要素にこれから発射するミサイルの情報を格納してflagをtrueにします。
	/// </remarks>
	/// <param name="p_position">プレイヤーの座標</param>
	/// <param name="lightDir">プレイヤーの前方</param>
	void missileShot(const CVector3 p_position, const CVector3 p_forward);

	/*!
	*@brief	ミサイルの制御。
	*/
	void missile_move(int missiletNumber);

	
	/*!
	*@brief	敵との当たり判定。
	*/
	void EnemyHit(Enemy* enemy);

	/*!
	*@brief	敵との当たり判定(マイン)。
	*/
	void EnemyHitMine(Enemy* enemy);

	/*!
	*@brief	敵との当たり判定(ブラックホール)。
	*/
	void EnemyHitBlackhole(Enemy* enemy);

	/*!
	*@brief	インスタンスの取得。
	*/
	static Player_BulletManager& Instance()
	{
		static Player_BulletManager instance;//Player_BulletManagerのインスタンスを生成。
		return instance;
	}
	
	/*!
	*@brief ブラックホールの敵弾吸収。
	*@param[in] e_bullet    カメラの右方向の単位ベクトル。
	*@return 吸い込む移動ベクトル。
	*/
	CVector3 Blackhole_EnemyBullet(CVector3 e_bullet);
	//不要な敵弾の削除。
	void erasebullet();
	//不要なプレイヤーの弾の全削除。
	void erasebullet_All();
	//敵弾との当たり判定。
	bool E_Bullet_toHit(CVector3 b_pos);
private:
	static const int Player_Missile_NUM = 10;      //ミサイルの配列の数。	
	int count = 0;
	int minecount = 0;
	int blackholecount = 0;
	int missile_ammo_NUM;//ミサイルの弾薬の数。   
	Player_MissileState missile[Player_Missile_NUM];//ミサイルの配列。

	Effekseer::Effect* m_sampleEffect = nullptr;
	Effekseer::Effect* m_blackholeEffect = nullptr;
	Effekseer::Handle m_mineEffectHandle = -1;

	//プレイヤーの弾を格納する。
	std::list<Player_BulletState*>P_BulletList;
	//プレイヤーのマインを格納する。
	std::list<Player_MineState*>P_MineList; 
	//プレイヤーのマインを格納する。
	std::list<Player_Blackhole*>P_BlackholeList;
	//Effekseer::Handle m_pBulletEffectHandle = -1;
};

//Player_BulletManagerのインスタンスを取得。
static inline Player_BulletManager& G_Player_BulletManager()
{
	return Player_BulletManager::Instance();
}
